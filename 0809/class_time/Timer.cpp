#include "Timer.h"

Timer::Timer()
{
    if((timerfd_ = ::timerfd_create(CLOCK_REALTIME, 0)) == -1)
    {
        ERR_EXIT("timerfd_create");
    }
    memset(&time_, 0 ,sizeof(time_));//别忘了对结构体初始化
}

Timer::~Timer()
{
    ::close(timerfd_);
}

void Timer::Settime(int interval, int now)
{
    time_.it_interval.tv_sec = interval;
    time_.it_value.tv_sec = now;
}

void Timer::Setcallback(const TimeCallback &callback)
{
    callback_ = callback;
}

void Timer::Run()
{
    struct pollfd event[1];
    event[0].fd = timerfd_;
    event[0].events = POLLIN;
    char buf[1024] = {0};
    int ret;

    if(timerfd_settime(timerfd_, 0, &time_, NULL) == -1)
        ERR_EXIT("timerfd_settime");
    while(1)
    {
        ret = poll(event, 1, -1);
        if(ret == -1)
        {
            if(EINTR == errno)
                continue;
            ERR_EXIT("poll");
        }
        else if(ret == 0)
        {
            std::cout << " timeout" << std::endl;
        }
        else
        {
            int nread = read(timerfd_, buf, 1024);
            if(nread == -1)
                    ERR_EXIT("read");
            callback_();
        }
    }
}
