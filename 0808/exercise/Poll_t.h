#ifndef _POLL_T_H_
#define _POLL_T_H_

#include "NonCopyable.h"
#include "TcpConnection.h"
#include <map>
#include <functional>
#include <poll.h>

class Poll_t : private NonCopyable
{
    public:
        typedef TcpConnection::TcpConnectionCallback PollerCallbak;
        explicit Poll_t(int listenfd);
        ~Poll_t();

        void poll();
        void handleAccept();
        void handleData();

        void handleConnectionEvent(int peerfd);
        void handleMessageEvent(int peerfd);
        void handleCloseEvent(int i);
//运行TcpConnection类中定义的回调函数运行机制
        void processConnect(const TcpConnectionPtr &conn)
        {   conn->handleConnection(); }
        void processMessage(const TcpConnectionPtr &conn)
        {   conn->handleMessage(); }
        void processClose(const TcpConnectionPtr &conn)
        {   conn->handleClose(); }

        void setConnectionCallback(const PollerCallbak &cb)
        {   onConnect_ = cb; }
        void setMessageCallback(const PollerCallbak &cb)
        {   onMessage_ = cb; }
        void setCloseCallback(const PollerCallbak &cb)
        {   onClose_ = cb;  }
    private:
        struct pollfd event_[2048];
        int listenfd_;
        int maxi_;
        int nready_;
        std::map< int, TcpConnectionPtr> lists_;

        PollerCallbak onConnect_;
        PollerCallbak onMessage_;
        PollerCallbak onClose_;
        typedef std::map<int,  TcpConnectionPtr>::iterator TcpIterator;
};

#endif
