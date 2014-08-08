#include "TcpSever.h"
#include "Socket.h"
#include "Poll_t.h"
#include <iostream>
#include <functional>
#include <signal.h>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

//对用户不可见
class IgnoreSigpipe 
{
    public:
        IgnoreSigpipe()
        {
            if(::signal(SIGPIPE,SIG_IGN) == SIG_ERR)
                ERR_EXIT("signal");
        }
};

IgnoreSigpipe Obj; //全局变量,一直存在

TcpSever::TcpSever(const InetAddress &addr)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        ERR_EXIT("socket");
    }
    sock_.reset(new Socket(fd));
    sock_->setReusePort();
    sock_->bindAddress(addr);
    sock_->listen();
}

void TcpSever::start()
{
    poller_.reset(new Poll_t(sock_->fd()));
    poller_->setConnectionCallback(onConnect_);
    poller_->setMessageCallback(onMessage_);
    poller_->setCloseCallback(onClose_);
    while(1)
    {
        poller_->poll();
        poller_->handleAccept();
        poller_->handleData();
    }
}
