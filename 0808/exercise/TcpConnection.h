#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include "NonCopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Rio.h"
#include <string>
#include <memory>
#include <functional>

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : private NonCopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
    public:
        typedef std::function< void (const TcpConnectionPtr&) > TcpConnectionCallback;
        TcpConnection(int sockfd,
                      const InetAddress &localAddr,
                      const InetAddress &peerAddr);
        ~TcpConnection();

        int fd() const
        {   return sockfd_.fd(); }

        ssize_t readn(char *usrbuf, size_t n);
        ssize_t readLine(char *usrbuf, size_t n);
        ssize_t writen(const char *usrbuf, size_t n);
        void send(const std::string &s);
        std::string receive();
        void shutdown();

        void setConnectionCallback(const TcpConnectionCallback &cb)
        {   Connection_ = cb; }
        void setMessageCallback(const TcpConnectionCallback &cb)
        {   Message_ = cb; }
        void setCloseCallback(const TcpConnectionCallback &cb)
        {   Close_ = cb; }

        void handleConnection();
        void handleMessage();
        void handleClose();

        const InetAddress &getLocalAddr() const
        {   return localAddr_; }
        const InetAddress &getPeerAddr() const
        {   return peerAddr_; }
    private:
        Socket sockfd_;
        Rio buffer_;

        const InetAddress localAddr_;
        const InetAddress peerAddr_;

        TcpConnectionCallback Connection_;
        TcpConnectionCallback Message_;
        TcpConnectionCallback Close_;
};

#endif
