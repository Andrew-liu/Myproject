#include "Time_Thread.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)


void Foo::read_wcache() //打开服务器就进行读取
{
    ifstream in;
    in.close();
    in.clear();
    in.open(config_.return_wcache());
    if(!in)
    {
        ERR_EXIT("open cache.txt");
    }
    string line;
    //读取文件中的Node对
    while(getline(in, line), !in.eof())
    {
        if(line == string())
            continue;
        istringstream istr(line);
        string first_str;
        istr >> first_str;
        string second_str(line.begin() + first_str.size(), --line.end());
        search_.return_cache().put(first_str, second_str);
    }
    in.close();
}


