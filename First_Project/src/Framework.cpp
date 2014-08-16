#include "Framework.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
using namespace std;
using namespace placeholders;

class IgnoreSigpipe
{
    public:
        IgnoreSigpipe()
        {
            if(::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
                ERR_EXIT("signal");
        }

};
IgnoreSigpipe OBJ;


Framework::Framework(const InetAddress &addr)
    :server_(addr),
     pool_(1000, 4),
     foo_(search_)
{
    search_.open_file(config_.return_file());
    search_.read_file();
    search_.make_map();
    search_.write_file(config_.return_out());
    search_.make_index();
//    foo_.read_wcache();
//    foo_.RunCallback();
    server_.setConnection(bind(&Framework::onConnection, this, _1));
    server_.setMessage(bind(&Framework::onMessage, this, _1));
    server_.setClose(bind(&Framework::onClose, this, _1));
    pool_.addTask(bind(&Foo::RunCallback, &foo_));
}

Framework::~Framework()
{}

void Framework::start()
{
    pool_.start();
    server_.start();
}

void Framework::onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << endl;
    conn->send("Hello, welcome to Echo Server!!!\r\n");
}


void Framework::onMessage(const TcpConnectionPtr &conn)
{
    string s(conn->receive());
//    cout << s;
    //不要把recv任务放到线程池
    //conn->send(s);
    pool_.addTask(bind(&Framework::search, this, s, conn));
}

//把计算任务和tcp回发做成一个函数，交给线程池
void Framework::search(std::string &word, const TcpConnectionPtr &conn)
{
    string ret_s;
//    ret_s = search_.search_file(word);
    search_.make_queue(word);
    ret_s = search_.return_key();
//    ret_s = search_.search_cache(word);
    conn->send(ret_s);
}

void Framework::onClose(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << "Close" << endl;
    conn->shutdown();
}



