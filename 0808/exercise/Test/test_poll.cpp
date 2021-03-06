#include "TcpConnection.h"
#include "Poll_t.h"
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void onMessage(const TcpConnectionPtr &conn)
{
    conn->receive();
    conn->send("ACK");
}

 int main(int argc, const char *argv[])
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        ERR_EXIT("socket");
    Socket sock(fd);
    InetAddress addr(8888);
    sock.setReusePort();
    sock.bindAddress(addr);
    sock.listen();

    Poll_t poller(sock.fd());
    poller.setMessageCallback(&onMessage);
    while(1)
    {
        poller.poll();
        poller.handleAccept();
        poller.handleData();
    }
    return 0;
}
