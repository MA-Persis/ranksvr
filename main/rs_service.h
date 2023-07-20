#ifndef _RS_SERVICE_H_
#define _RS_SERVICE_H_

#include <stdio.h>
#include <pthread.h>

#include <vector>

#include "concurrent/rank_thread.h"

namespace rs {

class RsService {
 public:
  RsService() { Init(); }

  ~RsService() {
    delete[] m_thread_;
  }

  static RsService& GetRsService() {
    static RsService rs_service;
    return rs_service;
  }

  void Init();

  int Start();

  void Join();

  void Stop();

 private:
  pthread_t* m_thread_;

  std::vector<RankThread*> rank_threads_;
};

} // namespace rs

#endif