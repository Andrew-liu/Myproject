#include "Poll_t.h"
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
        event_[i].fd = -1;
    event_[0].fd = listenfd_;
    event_[0].events = POLLIN;
    maxi_ = 0;
    nready_ = 0;
}

Poll_t::~Poll_t()
{
    close(listenfd_);
}

void Poll_t::poll()
{
    int ret; 
    do
    {
        ret = ::poll(event_,maxi_ + 1, -1);
    }while(ret == -1 && errno == EINTR);//中断会继续进行;
    if(ret == -1)
        ERR_EXIT("poll");
    nready_ = ret;
//    cout << "poll" << endl;
}

void Poll_t::handleAccept()
{
    if(event_[0].revents & POLLIN)
    {
        int peerfd = accept(listenfd_, NULL, NULL);
        if(peerfd == -1)
            ERR_EXIT("accept");
        handleConnectionEvent(peerfd);
    }
//    cout << "accept" << endl;
}

void Poll_t::handleData()
{
    for(int i = 1; i <= maxi_; ++i)
    {
        int peerfd = event_[i].fd;
//        cout << peerfd << endl;
        if(peerfd == -1)
            continue;
        if(event_[i].revents & POLLIN)
        {
//            cout << "recv" << endl;
            char buf[1024];
            int ret = recv(peerfd, buf, 1024, MSG_PEEK);
//            cout << buf << endl;
            if(ret == -1)
            {
                ERR_EXIT("recv");
            }
            else if(ret == 0)
            {
                cout << "close" << endl;
                handleCloseEvent(i);
                continue;
            }
            else
            {
                handleMessageEvent(peerfd);
            }

        }
    }
}

void Poll_t::handleConnectionEvent(int peerfd)
{
    int i;
    for(i = 1; i < 2048; i++)
    {
//        cout << "add" << endl;
       if(event_[i].fd == -1)
       {
            event_[i].fd = peerfd;
            event_[i].events = POLLIN;
            if(i > maxi_)
                maxi_ = i;
            break;
       }
    }
    if(i == 2048)
    {
        cout << "client close !!" << endl;
        fprintf(stderr, "too many client\n");
    }
    TcpConnectionPtr conn(new TcpConnection(peerfd,
       InetAddress::getLocalAddress(peerfd),
       InetAddress::getPeerAddress(peerfd)));
    //首先将三个回调函数注册到poll_t中的set函数中,
    //然后讲三个回调函数注册到tcpconnection中set函数
    //将pair <peerfd, ptr智能指针>放入map
    //进行回调函数管理




    //初始化建立连接的回调函数
    conn->setConnectionCallback(onConnect_);
    //消息处理的回调函数
    conn->setMessageCallback(onMessage_);
    //关闭连接
    conn->setCloseCallback(onClose_);
    pair<TcpIterator, bool > ret = lists_.insert(make_pair(peerfd, conn));
    if(ret.second == false)
        ERR_EXIT("map.insert");
    conn->handleConnection();//调用连接回调函数
}

void Poll_t::handleMessageEvent(int peerfd)
{
   TcpIterator it = lists_.find(peerfd);
   if(it != lists_.end())
   {
        it->second->handleMessage();//调用消息处理回调函数
   }
   else
        ERR_EXIT("list_.find")   ;
}

void Poll_t::handleCloseEvent(int i)
{
    //1.关闭描述符
    //2.重新初始化描述符
    //3.查找关闭回调函数
    //4.删除这个map元素
    int peerfd = event_[i].fd;
    close(peerfd);
    event_[i].fd = -1;
    TcpIterator it = lists_.find(peerfd);
    if(it != lists_.end())
    {
        //it返回的是map的元素.是一个pair
        it->second->handleClose();//调用关闭回调函数
        lists_.erase(it);
    }
    else
        ERR_EXIT("handleclose");
}
