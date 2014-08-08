#ifndef _TCPSEVER_H_
#define _TCPSEVET_H_

#include "NonCopyable.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "Poll_t.h"
#include <memory>

class TcpSever : private NonCopyable
{
    public:
        typedef TcpConnection::TcpConnectionCallback TcpCallback;
        TcpSever(const InetAddress &addr);
        
        void setConnection(const TcpCallback &cb)
        {   onConnect_ = cb; }
        void setMessage(const TcpCallback &cb)
        {   onMessage_ = cb; }
        void setClose(const TcpCallback &cb)
        {   onClose_ = cb; }

        void start();
    private:
        std::unique_ptr<Socket> sock_;
        std::unique_ptr<Poll_t> poller_;

        TcpCallback onConnect_;
        TcpCallback onMessage_;
        TcpCallback onClose_;
};

#endif
