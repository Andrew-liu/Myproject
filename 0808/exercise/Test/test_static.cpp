#include <echo/TcpServer.h>
#include <iostream>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void onMessage(const TcpConnectionPtr &conn)
{
    cout << conn->receive();
    conn->send("ack");
}

int main(int argc, const char *argv[])
{
    TcpServer  tcp(InetAddress(8888));
    tcp.setMessage(&onMessage);
    tcp.start();
    return 0;
}
