#ifndef _RANK_THREAD_H_
#define _RANK_THREAD_H_

#include <queue>
#include <vector>
#include <map>

#include <brpc/data_factory.h>
#include <bthread/bthread.h>

#include "base/rank.h"
#include "base/wait_group.h"

#include "util/timer.h"

#include "pb/echo.pb.h"

namespace rs {

static const int thread_numbers = 10;

struct TaskInfo {
  example::RankRequest* request;
  WaitGroup *wg;

  TaskInfo(example::RankRequest* _request,
           WaitGroup *_wg
          ) : 
          request(_request),
          wg(_wg) {
  }
  TaskInfo() : 
          request(nullptr), 
          wg(nullptr) {
  }
};

class RankThread {
public:
  RankThread() {
    init();
  }

  ~RankThread() {
    if (_rank_instance_list == NULL) return;

    std::map<uint32_t, RankType*>::iterator iter = _rank_instance_list->begin();
    for (;iter != _rank_instance_list->end(); ++iter) {
        delete iter->second;
    }

    delete _rank_instance_list;
    _rank_instance_list = NULL;  
  }

  enum Ad_Type {
    SEARCH=2,
    MOBILE_SEARCH=1024,
  };

  int init();

  static void* run(void* p);

  static int push(TaskInfo task);

  void handle();

public:
  static std::queue<TaskInfo> task_queue;
  static pthread_mutex_t m_mutex;
  static pthread_cond_t m_cond;

private:
  void process(example::RankRequest* request);
  int parallel_online_rank(example::RankStruct* rank_struct);
  void online_rank_extract_fea_part(example::RankStruct* rank_struct);
  void request_ms();
  void online_rank_compute_q(example::RankStruct* rank_struct);

  std::map<uint32_t, RankType*>* _rank_instance_list;
};


class RankThreadManager {
public:
  RankThreadManager() { init(); }

  void init() {
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);

    for (int i = 0; i < thread_numbers; i++) {
      example::RankStruct* rank_struct = new example::RankStruct();
      parallel_rank_struct.push_back(rank_struct);
    }
  }

  ~RankThreadManager() {
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);

    for (uint32_t i = 0; i < parallel_rank_struct.size(); i++) {
      delete parallel_rank_struct[i];
    }
  }

  void process_request(example::RankRequest* request);

  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
private:
  int parallel_online_rank(example::RankRequest* request);
  int split_req(std::vector<example::RankStruct*>* out,  example::RankRequest& request);
  void merge_rsp(std::vector<example::RankStruct*>* parts, example::RankRequest& request);

  example::RankStruct* _rank_struct;
  example::VisitInfo* _visit_info;
  example::QueryInfo* _query_info;

  std::vector<example::RankStruct*> parallel_rank_struct;

  WaitGroup _wg;
};


class SessionLocalDataFactory : public brpc::DataFactory {
public:
  void* CreateData() const {
      return new RankThreadManager;
  }

  void DestroyData(void* d) const {
      delete static_cast<RankThreadManager*>(d);
  }
};

} // namespace rs

#endif