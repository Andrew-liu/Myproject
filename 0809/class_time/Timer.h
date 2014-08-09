#ifndef _TIMER_H_
#define _TIMER_H_

#include "NonCopyable.h"
#include <functional>
#include <sys/timerfd.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <poll.h>

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

class Timer : private NonCopyable
{
    public:
        typedef std::function< void () > TimeCallback;
        Timer();
        ~Timer();

        void Settime(int interval, int now);
        void Setcallback(const TimeCallback &callback);
        void Run();
    private:
        int timerfd_;
        struct itimerspec time_;
        TimeCallback callback_;
};  

#endif
