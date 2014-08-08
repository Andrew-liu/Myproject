#ifndef _TEXTQUERY_H_
#define _TEXTQUERY_H_
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
typedef std::vector<std::string>::size_type line_no;
struct Fre_line
{
    int cnt; //存储词频
    std::set<line_no> nline;//存储不重复的行号
};
class TextQuery
{
    public:
        TextQuery();
        ~TextQuery();
        void open_file();//打开文件
        void read_file();//读取文件
        void change(std::string &line);
        void make_map();//建立map
        std::string search_file(std::string &s);//查询单词
        void sort_map(int size);
    private:
        std::ifstream in;
        std::vector<std::string> article_;
        std::map<std::string, Fre_line> word_;
};
int cmp(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b);
#endif
