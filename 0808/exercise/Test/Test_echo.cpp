#include <echo/TcpServer.h>
#include <iostream>
using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void onConnect(const TcpConnectionPtr &conn)
{
    conn->TcpConnection::send("A new client come in");    
}

void onMessage(const TcpConnectionPtr &conn)
{
    string ret = conn->receive();
    cout << ret << endl;
    for(string::iterator it = ret.begin(); it != ret.end(); ++it)
    {
        cout << *it << " " ;
    }
    cout << ret.size()  << endl;
    
    conn->TcpConnection::send(ret);
}

int main(int argc, const char *argv[])
{
    TcpServer tcp(InetAddress(8888));
    tcp.setConnection(&onConnect);
    tcp.setMessage(&onMessage);
    tcp.start();
    return 0;
}
