#ifndef _MULTISEARCH_H_
#define _MULTISEARCH_H_ 

#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include <echo/NonCopyable.h>
#include "TextQuery.h"

class MultiSearch : NonCopyable
{
    public:
        MultiSearch(const InetAddress &addr);
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);

        void search(std::string &word, const TcpConnectionPtr &conn);

        TcpServer server_;
        ThreadPool pool_;
        TextQuery search_;
};



#endif  /*ECHO_TCP_SERVER_H*/
