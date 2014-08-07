#include "Poll_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

Poll_t::Poll_t(int listenfd)
    :listenfd_(listenfd)
{
    for(int i = 0; i < 2048; ++i)
    {
        events_[i].fd = -1;
    }
    events_[0].fd = listenfd;
    events_[0].events = POLLIN;
    maxi_ = 0;
    nready = 0;
}

void Poll_t::poll()
{
    int ret ;
    do 
    {
        ret = ::poll(events_, maxi + 1, 10000);
    }while(ret == -1 && errno == EINTR);
    if(ret == -1)
        ERR_EXIT("poll");
    nready_ = ret; 
    cout << "POLL" << ret << endl;
}

void Poll_t::handleAccept()
{
    if(nready_ == 0)
        return ;
    if(events_[0].revents & POLLIN)
    {
        int peerfd = ::accept(listenfd_, NULL, NULL);
        if(peerfd == -1)
            ERR_EXIT("accept");
        //增加客户端的描述符
        //map增加一个新连接, 建立起tcp指针和描述符之间的关系
        //处理onConnection
        
        cout << "Accept a peerfd" << peerfd << endl;
        handleConnectionEvent(peerfd);
    }
}


void Poll_t::handleData()
{
    int i;
    char buf[100] = {0};
    for(int i = 1; i <= maxi_; ++i)
    {
        int peerfd = event_[i].fd;
        if(peerfd == -1)
            continue;
        cout << "handle" << peerfd << endl;
        if(event_[i].revents & POLLIN)
        {
            int ret = recv(peerfd, buf, sizeof(buf),MSG_PEEK);
            cout << "recv" << endl;
            if(ret == -1)
                ERR_EXIT("Recv");
            if(ret == 0)
            {
                handleCloseEvent(i);
            }
            else
                handleMessageEvents(peerfd);
        }
    }
}


void Poll_t::handleConnectionEvent(int peerfd)
{
    for(int i = 1; i < 2048; ++i)
    {
        if(event_[i].fd == -1)
        {
            event_[i].fd = peerfd;
            event_[i].events = POLLIN;
            if(i > maxi_)
                maxi = i;
            break;
        }
    }
    if(i == 2048)
    {
        fprintf(stderr, "too many argument\n");
        exit(EXIT_FAILURE);
    }

    //主要是后面的逻辑
    //创建一个智能指针
    TcpConnectionPtr con(new TcpConnection(peerfd));
    con->setConnectionCallback(onConnectionCallback_);
    con->setMessageCallback(onMessageCallback_);
    con->setCloseCloseback(onCloseCallback_);
    //加入map
    lists_.insert(make_pair(peerfd, con));
    con->handleConnection();
}

void Poll_t::handleMessageEvents(int peerfd)
{
    TcpConnectionIterator it = lists.find(peerfd);
    assert(it != list_.end());
    cout << "handle message" << peerfd << endl;
    //迭代器获取的是pair value是一个智能指针
    it->second->handleMessage();
}

void Poll_t::handleCloseEvent(int i)
{
    assert(i > 0 && i <2048);
    int peerfd = event_[i].fd;
    TcpConnectionIterator it = lists_.find(peerfd);
    assert(it != lists_.end());
    cout << "Handle Close" << peerfd << endl;
    it->second->handleClose();
    list_.erase(it);
    event_[i].fd = -1;
}


