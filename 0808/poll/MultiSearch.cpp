#include "MultiSearch.h"
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;
using namespace placeholders;

MultiSearch::MultiSearch(const InetAddress &addr)
    :server_(addr),
     pool_(1000, 4)
{
    search_.open_file();
    search_.read_file();
    search_.make_map();
    server_.setConnection(bind(&MultiSearch::onConnection, this, _1));
    server_.setMessage(bind(&MultiSearch::onMessage, this, _1));
    server_.setClose(bind(&MultiSearch::onClose, this, _1));
}

void MultiSearch::start()
{
    pool_.start();
    server_.start();
}

void MultiSearch::onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << endl;
    conn->send("Hello, welcome to Echo Server!!!\r\n");
}


void MultiSearch::onMessage(const TcpConnectionPtr &conn)
{
    string s(conn->receive());
    cout << s;
    //不要把recv任务放到线程池
    //conn->send(s);
    pool_.addTask(bind(&MultiSearch::search, this, s, conn));
}

//把计算任务和tcp回发做成一个函数，交给线程池
void MultiSearch::search(std::string &word, const TcpConnectionPtr &conn)
{
    string ret_s;
    ret_s = search_.search_file(word);
    conn->send(ret_s);
}

void MultiSearch::onClose(const TcpConnectionPtr &conn)
{
    cout << conn->getPeerAddr().toIp() << " port: "
        << conn->getPeerAddr().toPort() << "Close" << endl;
    conn->shutdown();
}



