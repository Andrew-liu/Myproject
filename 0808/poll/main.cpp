#include "MultiSearch.h"
using namespace std;

int main(int argc, const char *argv[])
{
    MultiSearch server(InetAddress(8989));
    server.start();
    return 0;
}
