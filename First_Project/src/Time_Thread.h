#ifndef _TIME_THREAD_
#define _TIME_THREAD_

#include <echo/Thread.h>
#include <echo/Timer.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Foo
{
    public://在线程中运行定时器
        Foo(): thread_(bind(&Timer::runTimer, &time_))
        {
        }
        void print(){ cout << "Hello World" << endl;}
        void RunCallback()
        {
            time_.setTimer(1, 3);
            time_.setTimerCallback(bind(&Foo::print, this));
            thread_.start();
            thread_.join();

        }
    private:
        Timer time_;
        Thread thread_;
};

#endif
