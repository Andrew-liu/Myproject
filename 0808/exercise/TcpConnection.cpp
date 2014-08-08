#include "TcpConnection.h"

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

TcpConnection::TcpConnection(int sockfd,
               const InetAddress &localAddr,
               const InetAddress &peerAddr)
              :sockfd_(sockfd),
               buffer_(sockfd),
               localAddr_(localAddr),
               peerAddr_(peerAddr)
{}

TcpConnection::~TcpConnection()
{
    shutdown();
}

ssize_t TcpConnection::readn(char *usrbuf, size_t n)
{
    ssize_t ret = buffer_.readn(usrbuf, n);
    if(ret == -1)
        ERR_EXIT("readn");
    return ret;
}

ssize_t TcpConnection::readLine(char *usrbuf, size_t n)
{
    ssize_t ret = buffer_.readline(usrbuf, n);
    if(ret == -1)
        ERR_EXIT("readLine");
    return ret;
}

ssize_t TcpConnection::writen(const char *usrbuf, size_t n)
{
    ssize_t ret = buffer_.writen(usrbuf, n);
    if(ret == -1)
        ERR_EXIT("writen");
    return ret;
}

void TcpConnection::send(const string &s)
{
    writen(s.c_str(), s.size());//调用类内部包装过的writen
}

string TcpConnection::receive()
{
    char buf[1024] = {0};
    readLine(buf, 1024);
    return string(buf);
}

void TcpConnection::shutdown()
{
    sockfd_.shutdownWrite();
}

//调用回调函数
void TcpConnection::handleConnection()
{
    if(Connection_)//不为空的时候才去调用,为空的时候直接跳过
        Connection_(shared_from_this());
}

void TcpConnection::handleMessage()
{
    if(Message_)
        Message_(shared_from_this());
}

void TcpConnection::handleClose()
{
    if(Close_)
        Close_(shared_from_this());
}
