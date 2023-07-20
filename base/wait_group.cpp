// date: 2018-12-25 20:22:09
#include "wait_group.h"

#include <sys/time.h>

namespace rs {

WaitGroup::WaitGroup() : _counter(0) {
    bthread_mutex_init(&_mutex, NULL);
    bthread_cond_init(&_cond, NULL);
}

WaitGroup::~WaitGroup() {
    bthread_mutex_destroy(&_mutex);
    bthread_cond_destroy(&_cond);
}

void WaitGroup::clear() {
    bthread_mutex_lock(&_mutex);
    _counter = 0;
    bthread_cond_broadcast(&_cond);
    bthread_mutex_unlock(&_mutex);
}

void WaitGroup::add(int delta) {
    if (delta == 0) {
        return;
    }

    bthread_mutex_lock(&_mutex);
    _counter += delta;
    if (_counter == 0) {
        bthread_cond_broadcast(&_cond);
    }
    bthread_mutex_unlock(&_mutex);
}

bool WaitGroup::wait(int timeout_ms) {
    bthread_mutex_lock(&_mutex);

    if (_counter != 0) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        long long absmsec = tv.tv_sec * 1000ll + tv.tv_usec / 1000ll + timeout_ms;
        struct timespec abstime;
        abstime.tv_sec = absmsec / 1000ll;
        abstime.tv_nsec = (absmsec % 1000ll) * 1000000ll;
        bthread_cond_timedwait(&_cond, &_mutex, &abstime);
    }
    bool ret = (_counter == 0);
    bthread_mutex_unlock(&_mutex);

    return ret;
}

void WaitGroup::wait() {
    bthread_mutex_lock(&_mutex);
    while (_counter != 0) {
        bthread_cond_wait(&_cond, &_mutex);
    }
    bthread_mutex_unlock(&_mutex);
}

} // namespace rs

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
