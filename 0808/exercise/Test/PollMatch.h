#ifndef _POLLMATCH_H_
#define _POLLMATCH_H_

#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include <echo/NonCopyable.h>

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

class PollMatch : private NonCopyable
{
    public:
        PollMatch(const InetAddress &addr);
        ~PollMatch();
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);

        void match(std::string &s, const TcpConnectionPtr &conn);
        
        TcpServer tcp_;
        ThreadPool pool_;
};

#endif
