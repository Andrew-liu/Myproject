#ifndef _POLL_T_H
#define _POLL_T_H

#include "NonCopyable.h"
#include "TcpConnection.h"
#include <map>
#include <poll.h>

class PollPoller : private NonCopyable
{
    public:
        typedef TcpConnection::TcpConnectionCallback PollerCallback;
        explicit PollPoller(int listenfd);
        void poll();
        void handleAccept();
        void handleData();
        
        void setConnectionCallback(const PollerCallback &cb)
        {
            onConnectionCallback_ = cb;
        }
    private:
        struct pollfd event_[2048];
        int listenfd_;
        int maxi_;
        int nready_;
        std::map<int, TcpConnectionPtr> list_;
        //从fd到TcpConnection的映射
        typedef std::map<int, TcpConnectionPtr>::iterator TcpConnectionIterator;
}

#endif
