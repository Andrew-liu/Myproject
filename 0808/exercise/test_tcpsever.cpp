#include "TcpSever.h"
#include <iostream>
using namespace std;
//IgnoreSigpipe obj;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void onMessage(const TcpConnectionPtr &conn)
{
    cout << conn->receive();
    conn->send("ACK");
}

int main(int argc, const char *argv[])
{
    InetAddress addr(8888);
    TcpSever tcp(addr);
    tcp.setMessage(&onMessage);
    tcp.start();
    return 0;
}
