#include "TcpServer.h"
#include <iostream>
#include <string>
using namespace std;

void onConnect(const TcpConnectionPtr &conn)
{
    conn->send("hello\r\n");
    conn->send("the TCP Connection will be closed!\r\n");
//    sleep(100);
}

void onMessage(const TcpConnectionPtr &conn)
{
    cout << "recv data" << conn->receive();
    conn->send("ACK\r\n");
}




int main(int argc, const char *argv[])
{
    InetAddress addr(8989);
    TcpServer server(addr);
    server.setConnection(&onConnect);
    server.setMessage(&onMessage);
    server.start();
    return 0;
}











