#include "PollPoller.h"
#include "TcpConnection.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

int get_listen_fd();

void onConnection(const TcpConnectionPtr &conn)
{
    conn->send("hello");
}

void onMessage(const TcpConnectionPtr &conn)
{
    char buf[1024];
    conn->readLine(buf, 1024);
    printf("%s", buf);
    conn->send(buf);
}

void onClose(const TcpConnectionPtr &conn)
{
    conn->shutdown();
}


int main(int argc, const char *argv[])
{
    int listenfd = get_listen_fd();
    if(listenfd == -1)
        ERR_EXIT("listenfd");

    PollPoller poller(listenfd);
    poller.setConnectionCallback(onConnection);
    poller.setMessageCallback(onMessage);
    poller.setCloseCallback(onClose);
    while(1)
    {
        poller.poll();
        poller.handleAccept();
        poller.handleData();
    }
    
    return 0;
}

int get_listen_fd()
{
    //创建socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
        ERR_EXIT("socket");


    //设置端口复用
    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8989);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind端口
    if(::bind(listenfd, (struct sockaddr*)&servaddr, sizeof servaddr) < 0)
        ERR_EXIT("bind"); 

    //listen端口
    if(listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

    return listenfd;
}


