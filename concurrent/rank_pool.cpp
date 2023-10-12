#include "concurrent/rank_pool.h"

namespace rs {

bthread_mutex_t RankPool::mutex_;
std::stack<RankThread*> RankPool::stack_;

RankThread* RankPool::pop_rank_thread() {
  bthread_mutex_lock(&mutex_);
  if (!stack_.empty()) {
    RankThread* rank_thread = stack_.top();
    stack_.pop();
    bthread_mutex_unlock(&mutex_);
    return rank_thread;
  }
  bthread_mutex_unlock(&mutex_);

  RankThread* rank_thread = new RankThread();
  return rank_thread;
}

void RankPool::push_rank_thread(RankThread* rank_thread) {
  std::lock_guard<bthread_mutex_t> lock(mutex_);
  stack_.push(rank_thread);
}

} // namespace rs
