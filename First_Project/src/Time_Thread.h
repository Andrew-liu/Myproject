#ifndef _TIME_THREAD_
#define _TIME_THREAD_

#include "TextQuery.h"
#include "Config.h"
#include <echo/Thread.h>
#include <echo/Timer.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>


class Foo
{
    public://在线程中运行定时器
        Foo(TextQuery &search)
            : thread_(bind(&Timer::runTimer, &time_)),
              search_(search)
        {
        }

        void read_wcache();
        void write_wcache()
        {    search_.write_cache(); 
             cout << "write cache successful!" << endl;
        }
        void RunCallback()
        {
            time_.setTimer(1, 3);
            time_.setTimerCallback(bind(&Foo::write_wcache, this));
            thread_.start();
            thread_.join();

        }
    private:
        Timer      time_;
        Thread     thread_;
        TextQuery &search_;
        Config     config_;
};

#endif
