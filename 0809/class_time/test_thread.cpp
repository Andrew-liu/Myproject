#include "Thread.h"
#include "Timer.h"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Foo
{
    public://在线程中运行定时器
        Foo(): thread_(bind(&Timer::Run, &time_))
        {
        }
        void print(){ cout << "Hello World" << endl;}
        void RunCallback()
        {
            time_.Settime(1, 3);
            time_.Setcallback(bind(&Foo::print, this));
            thread_.start();
            thread_.join();

        }
    private:
        Timer time_;
        Thread thread_;
};

int main(int argc, const char *argv[])
{
    Foo foo;
    foo.RunCallback();
    return 0;
}
