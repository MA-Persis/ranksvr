#ifndef RANK_POOL_H_
#define RANK_POOL_H_

#include <stack>

#include <bthread/mutex.h>

#include "concurrent/rank_thread.h"

namespace rs {

class RankThread;

class RankPool {
 public:
  RankPool() {
    bthread_mutex_init(&mutex_, NULL);
  }

  ~RankPool() {
    bthread_mutex_destroy(&mutex_);

    while (!stack_.empty()) {
      RankThread* rank_thread = stack_.top();
      stack_.pop();
      delete rank_thread;
    }
  }

  static RankPool& instance() {
    static RankPool rank_pool;
    return rank_pool;
  }

  RankThread* pop_rank_thread();

  void push_rank_thread(RankThread* rank_thread);

 private:
  static bthread_mutex_t mutex_;
  static std::stack<RankThread*> stack_;
};

} // namespace rs

#endif