#include "concurrent/rank_thread.h"

#include "conf/rs_conf.h"

#include <json2pb/pb_to_json.h>

namespace rs {

std::queue<TaskInfo> RankThread::task_queue;

pthread_mutex_t RankThread::m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t RankThread::m_cond = PTHREAD_COND_INITIALIZER;

static void* rank_process(void* args) {
    TaskInfo* task_info = static_cast<TaskInfo*>(args);

    RankThread* rank_thread = RankPool::instance().pop_rank_thread();

    rank_thread->process(task_info->rank_struct,
                         task_info->rank_request,
                         task_info->irequest);

    RankPool::instance().push_rank_thread(rank_thread);

    return nullptr;
}

int RankThread::init() {
  LOG(NOTICE) << "start searchthread";
  _rank_instance_list = new std::map<uint32_t,RankType *>;

  rs::RsConfig &config = rs::RsConfig::get_cur_config();

  for(size_t i = 0; i < config.m_rankInfoList.size(); i++)
  {
    RankType *rank_instance = NULL;
    CreateRanking(&rank_instance);
    if(rank_instance == NULL) {
      LOG(FATAL) << "CreateRanking Failed";
      return -1;
    }

    if(!rank_instance->Init(config.m_rankInfoList[i].rank_conf)) {
        LOG(FATAL) << "InitRanking Failed";
        return -1;
    }

    (*_rank_instance_list)[config.m_rankInfoList[i].rank_type] = rank_instance;
  }

  return 0;
}

void RankThread::process(example::RankStruct* rankstruct, example::RankRequest* request, iRequest* irequest) {
  example::RankStruct* rank_struct = request->mutable_rank_struct(0);
  parallel_online_rank(rank_struct);
}

int RankThread::parallel_online_rank(example::RankStruct* rank_struct) {
  online_rank_extract_fea_part(rank_struct);

  request_ms();

  online_rank_compute_q(rank_struct);

  return 0;
}

void RankThread::online_rank_extract_fea_part(example::RankStruct* rank_struct) {
  RankType* ranker = (*_rank_instance_list)[SEARCH];

  int ret = ranker->GetFeatureKey(*rank_struct);
  if (ret != 0) {
    LOG(FATAL) << "OnlineRank GetFeatureKey failed";
  }
}

void RankThread::request_ms() {
  bthread_usleep(1000);
}

void RankThread::online_rank_compute_q(example::RankStruct* rank_struct) {
  RankType* ranker = (*_rank_instance_list)[SEARCH];

  int ret = ranker->DoRank(*rank_struct);
  if (ret != 0) {
    LOG(FATAL) << "OnlineRank DoRank failed";
  }
}

void RankThreadManager::process_request(example::RankRequest* request) {
  // std::string request_json;
  // json2pb::ProtoMessageToJson(*request, &request_json);
  // LOG(NOTICE) << "request_json:" << request_json;

  int non_empty_struct = 0;

  for (int i = 0; i < request->rank_struct_size(); i++) {
    if (request->rank_struct(i).querylist_size() > 0
        && request->rank_struct(i).querylist(0).adlist_size() > 0) {
      _rank_struct = request->mutable_rank_struct(i);
      _query_info = _rank_struct->mutable_querylist(0);
      _visit_info = _rank_struct->mutable_visitinfo();
    }
    non_empty_struct++;
  }

  if (non_empty_struct > 1) {
    LOG(FATAL) << "online data detect more than 1 non empty rank_struct";
  }

  parallel_online_rank(request);
}

int RankThreadManager::parallel_online_rank(example::RankRequest* request) {
  std::vector<example::RankStruct*> parts;
  split_req(&parts, *request);

  int parts_size = parts.size();
  qihoo::ad::TimeDelta tasks_process_timer;

  std::vector<bthread_t> bids(parts_size);

  TaskInfo task_info[parts_size];
  iRequest irequest[parts_size];

  for (int i = 0; i < parts_size; i++) {
    task_info[i] = TaskInfo(parts[i], request, &irequest[i]);
    if (bthread_start_background(&bids[i], NULL, rank_process, &task_info[i]) != 0) {
        LOG(FATAL) << "fail to create bthread to process rank";
        return -1;
    }
  }

  for (int i = 0; i < parts_size; i++) {
      bthread_join(bids[i], NULL);
  }

  // LOG(INFO) << "tasks_process_timer: " << tasks_process_timer.GetSysTimeDelta();

  merge_rsp(&parts, *request);

  return 0;
}

int RankThreadManager::split_req(std::vector<example::RankStruct*>* out,  example::RankRequest& request) {
  int ad_num = _query_info->adlist_size();
  int thread_num = 2;
  int per_thread = ad_num / thread_num;
  int remnant = ad_num - per_thread * thread_num;

  example::QueryInfo query_info;
  query_info.mutable_adlist()->Swap(_query_info->mutable_adlist());

  for (int thrd_idx = 0; thrd_idx < thread_num; thrd_idx++) {
    example::RankStruct* part = parallel_rank_struct[thrd_idx];
    part->Clear();
    out->push_back(part);
    // 需要完整构造rank_struct
    part->mutable_visitinfo()->CopyFrom(*(_visit_info));

    example::QueryInfo* sub_query_info = part->add_querylist();
    sub_query_info->CopyFrom(*(_query_info));
    sub_query_info->mutable_adlist()->Clear();

    // 分配广告
    for (int ad_idx = 0; ad_idx < per_thread; ad_idx++) {
      example::AdInfo4Rank* adinfo = query_info.mutable_adlist()->ReleaseLast();
      sub_query_info->mutable_adlist()->AddAllocated(adinfo);
    }
    if (remnant > 0) {
      example::AdInfo4Rank* adinfo = query_info.mutable_adlist()->ReleaseLast();
      sub_query_info->mutable_adlist()->AddAllocated(adinfo);
      remnant--;
    }

    // std::string part_json;
    // json2pb::ProtoMessageToJson(*part, &part_json);
    // LOG(NOTICE) << "part_request_json:" << part_json;
  }
  return 0;
}

void RankThreadManager::merge_rsp(std::vector<example::RankStruct*>* parts, 
                                  example::RankRequest& request) {
  int part_size = parts->size();
  for (uint32_t i = 0; i < part_size; i++) {
    example::RankStruct* rank_struct = (*parts)[i];
    if (rank_struct->querylist_size() > 0) {
      example::QueryInfo* query_info = rank_struct->mutable_querylist(0);

      int ad_num = query_info->adlist_size();
      for (int j = 0; j < ad_num; j++) {
          example::AdInfo4Rank* adinfo = query_info->mutable_adlist()->ReleaseLast();
          _query_info->mutable_adlist()->AddAllocated(adinfo);
      }
    }
  }

  // std::string json;
  // json2pb::ProtoMessageToJson(*_rank_struct, &json);
  // LOG(NOTICE) << "merge_rsp_json:" << json;
}

} // namespace rs