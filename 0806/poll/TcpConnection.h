#ifndef _TCPCONNECT_H_
#define _TCPCONNECT_H_

#include "NonCopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Rio.h"
#include <string.h>
#include <memory>
#include <functional>
#include <iostream>

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr; //给只能指针的类型起一个别名

class TcpConnection : private NonCopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
    public:
        typedef std::function< void (const TcpConnectionPtr &) > TcpConnectionCallback;

        explicit TcpConnection(int sockfd);
        ~TcpConnection();

        int fd() const 
        {   return sockfd_.fd;  }

        ssize_t readn(char *usrbuf, size_t n);
        ssize_t readLine(char *usrbuf, size_t maxline);
        ssize_t writen(const char *usrbuf, size_t n);

        void send(const std::string &s);
        void shutdown();
//设置三个回调函数
        void setConnectionCallback(const TcpConnectionCallback &cb)
        {   onConnectionCallback_ = cb; }
        void setMessageCallback(const TcpConnectionCallback &cb)
        {   onMessageCallback_ = cb; }
        void setConnectionCallback(const TcpConnectionCallback &cb)
        {   onCloseCallback_ = cb;  }


//启动三个回调函数        
        void handleConnection()
        {   onConnectionCallback_(share_from_this()); }
        viod handleMessage()
        {
            std::cout << "on TcpConnectionCallback Message " << std::endl;
            onMessageCallback_(share_from_this());
        }
        void handleClose()
        {   onCloseCallback_(share_from_this());    }
    private:
        Socket socket_;
        Rio buffer_;
        
//三个回调函数,分别是建立时的,消息到达和连接关闭的回调
        TcpConnectionCallback onConnectionCallback_;
        TcpConnectionCallback onMessageCallback_;
        TcpConnectionCallback onCloseCallback_;
};
#endif
