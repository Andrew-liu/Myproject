#ifndef RIO_H
#define RIO_H 

#include "NonCopyable.h"
#include <stdio.h>
#define MAXLINE 1024
#define RIO_BUFFER 8192

class Rio : private NonCopyable
{
    public:
        explicit Rio(int fd);        

        ssize_t readn(char *usrbuf, size_t n);
        ssize_t readline(char *usrbuf, size_t n);
        ssize_t writen(const char *usrbuf, size_t n);

    private:
        ssize_t read(char *usrbuf, size_t n);

        int fd_;        //读取或者写入的文件描述符
        int left_;      //要读取的字符串的长度
        char *bufptr_;         //指向要读的字符串的首指针
        char buffer_[RIO_BUFFER]; //缓冲区
};

#endif  /*RIO_H*/
