#include "PollMatch.h"
#include <iostream>
#include <string>

using namespace std;
int main(int argc, const char *argv[])
{
    PollMatch match(InetAddress(8888));
    match.start();
    return 0;
}
