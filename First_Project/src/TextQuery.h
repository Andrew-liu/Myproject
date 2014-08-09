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

#define MAXSIZE 100
typedef std::vector<std::string>::size_type line_no;

struct Key_word
{
    string word;
    int distance;
    int cnt;
}
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

        void open_file(std::string s);//打开文件
        void read_file();//读取文件
        void change(std::string &line);
        void make_map();//建立map
        std::string search_file(std::string &s);//查询单词

        int edit_distance(const std::string &s1, const std::string &s2);
        void sort_map(int size);
    private:
        std::ifstream                   in;
        std::vector<std::string>        article_;
        std::map<std::string, Fre_line> word_;
        int  memo_[MAXSIZE][MAXSIZE];
        Key_word  key_;
};
int cmp(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b);
#endif
