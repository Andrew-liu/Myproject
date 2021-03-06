#ifndef _FRAEMWORK_H_
#define _FREAMWORK_H_

#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include <echo/NonCopyable.h>
#include "Time_Thread.h"
#include "TextQuery.h"
#include "Config.h"
#include <iostream>

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

class Framework : private NonCopyable
{
    public:
        Framework(const InetAddress &addr);
        ~Framework();
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);
        void onClose(const TcpConnectionPtr &conn);
        void search(std::string &word, const TcpConnectionPtr &conn);

        TcpServer  server_;
        ThreadPool pool_;
        TextQuery  search_;
        Config     config_;
        Foo        foo_;
};

#endif
