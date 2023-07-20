#include "main/rs_service.h"

namespace rs {

static const int thread_num = 16;

void RsService::Init() {
  m_thread_ = new pthread_t[thread_num];
}

int RsService::Start() {
  LOG(INFO) << "rank_thread service start.";

  RankThread* pThread = new RankThread[thread_num];
  for (uint32_t i=0; i < thread_num; ++i) {
    
    int nRet = pthread_create(&m_thread_[i], NULL, RankThread::run, &pThread[i]);

    if (nRet) {
      LOG(INFO) << "pthread create failed";
      return -1;
    }

    rank_threads_.push_back(&pThread[i]);
  }
  
  return 0;
}

void RsService::Join() {
  for (uint32_t i=0; i < thread_num; ++i) {
    pthread_join(m_thread_[i], nullptr);
  }
}

void RsService::Stop() {
  for (auto& rt : rank_threads_) {
    delete rt;
  }
}

} // namespace rs