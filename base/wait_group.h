// date: 2018-12-25 20:21:29
#ifndef __BASE_WAIT_GROUP_H__
#define __BASE_WAIT_GROUP_H__

#include <pthread.h>
#include <bthread/bthread.h>
#include <bthread/mutex.h>
#include <bthread/condition_variable.h>

namespace rs {

class WaitGroup {
public:
    WaitGroup();
    ~WaitGroup();
    void clear();
    void add(int delta);
    void done() {
        add(-1);
    }
    // 不一定是精确的timeout_ms毫秒
    // 因为没有处理被信号打断的情况
    // 现在不要求精确，可以先这么用着
    bool wait(int timeout_ms);
    void wait();

private:
    bthread_mutex_t _mutex;
    bthread_cond_t _cond;
    int _counter;

private:
    WaitGroup(const WaitGroup&);
    WaitGroup& operator=(const WaitGroup&);
};

} // namespace rs

#endif //__BASE_WAIT_GROUP_H__
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
