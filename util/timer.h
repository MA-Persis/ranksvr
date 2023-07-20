#ifndef _QIHOO_AD_RANKING_TIMER_H_
#define _QIHOO_AD_RANKING_TIMER_H_

#include<sys/time.h>
#include<unistd.h>

namespace qihoo{ namespace ad{

class Timer{
public:
    struct timeval start_;
    struct timeval end_;
    size_t timeuse_;

    Timer(): timeuse_(0) {
    }

    void clear() {
        timeuse_ = 0;
    }

    void start() {
        gettimeofday( &start_, NULL );
    }

    void end() {
        gettimeofday( &end_, NULL );
        size_t cost = 1000000 * ( end_.tv_sec - start_.tv_sec ) + end_.tv_usec - start_.tv_usec;
        timeuse_ += cost;
    }

    timeval & get_start() {
        return start_;
    }

    timeval & get_end() {
        return end_;
    }

    size_t cost() {
        return timeuse_;
    }

    size_t get_diff(const timeval &start,const timeval &end) {
        return  1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    }

    float cost_sec() {
        return timeuse_ / 1000000.0;
    }
};

class TimeDelta
{
public:
    TimeDelta()
    {
        _sys_start = GetSysTime();
        _cpu_start = GetCPUTime();
    }

    ~TimeDelta()
    { }

    int64_t GetSysTimeDelta() const 
    {
        return GetSysTime() - _sys_start;
    }

    int64_t GetCpuTimeDelta() const 
    {
        return (GetCPUTime() - _cpu_start) / CLOCKS_PER_SEC;
    }

    void Reset()
    {
        _sys_start = GetSysTime();
        _cpu_start = GetCPUTime();
    }

    static long GetSysTime()
    {
        struct timeval t;
        gettimeofday(&t, NULL);

        return t.tv_sec * 1000 + t.tv_usec / 1000;
    }

    static clock_t GetCPUTime()
    {
        return clock();
    }

private:
    int64_t _sys_start;
    clock_t _cpu_start;
};

} } // namespace

#endif

