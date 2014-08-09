#include "Timer.h"
using namespace std;

void print()
{
    cout << "fuck you!!!!" << endl;
}

int main(int argc, const char *argv[])
{
    Timer t;
    t.Settime(1, 3);
    t.Setcallback(print);
    t.Run();
    return 0;
}
