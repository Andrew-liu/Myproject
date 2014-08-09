#include "PollMatch.h"
#include <iostream>
#include <string>
using namespace std;
using namespace placeholders;

PollMatch::PollMatch(const InetAddress &addr)
    :tcp_(addr),
     pool_(1000,4)
{
    tcp_.setConnection(bind(&PollMatch::onConnection, this, _1));
    tcp_.setMessage(bind(&PollMatch::onMessage, this, _1));
    tcp_.setClose(bind(&PollMatch::onClose, this, _1));
}

PollMatch::~PollMatch()
{}

void PollMatch::start()
{
    pool_.start();//线程池要先开启,然后才能放任务
    tcp_.start();//tcp_一开启就开始放任务
}

void PollMatch::onConnection(const TcpConnectionPtr &conn) 
{
    conn->send("welcome to this server");
}

void PollMatch::onMessage(const TcpConnectionPtr &conn)
{
    string s = conn->receive();
    //将任务加入线程池
    cout << s << endl;
    pool_.addTask(bind(&PollMatch::match, this, s, conn));
}

void PollMatch::match(string &s, const TcpConnectionPtr &conn)
{
    cout << s.size() << endl;
    cout << "add /r/n " << endl;
    conn->send("check successful!");
}

void PollMatch::onClose(const TcpConnectionPtr &conn)
{
    conn->shutdown();
}
