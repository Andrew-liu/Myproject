#include "TextQuery.h"
#include <string.h>
using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

TextQuery::TextQuery()
{
    memset(key_, 0, sizeof(key));
}

TextQuery::~TextQuery(){}

void TextQuery::open_file(string s)
{
    in.close();
    in.clear();
    in.open(s.c_str());
    if(!in)
    {
        ERR_EXIT("open");
    }
}

//vector存储整个文章,一个string存储一行
void TextQuery::read_file()
{
    string line;
    while(getline(in,line),!in.eof())
    {
        change(line);
        article_.push_back(line);
//        cout << line << endl;
    }
}
void TextQuery::change(string &line)
{
    for(string::iterator it = line.begin(); it != line.end(); ++it)
    {
        if(isupper(*it))
        {
            *it = tolower(*it);
        }
        else if(ispunct(*it))
        {
            *it = ' ';
        }
        else if(isdigit(*it))
        {
            *it = ' ';
        }
        else
        {
            continue;
        }
    }
}

void TextQuery::make_map()
{
    string word;
    for(vector<string>::size_type ix = 0; ix != article_.size(); ++ix)
    {
        istringstream s_in(article_[ix]);
        while(s_in >> word)
        {
            ++ (word_[word].cnt);//词频加一
            word_[word].nline.insert(ix); //行号加入set
        }
    }
}

string TextQuery::search_file(string &s)
{
    pair<string, int> only;
    string word(s.begin(), ----s.end());
    int min = 3, temp;
    for(map<string, Fre_line>::iterator it = map.begin(); it != map.end(); ++it)
    {
        temp = edit_distance(word, it->first);
        if(temp > 3)
            continue;
       
            if(temp < min)//编辑距离小于最小值的时候, 保存单词和词频
            {
                min = temp;
                only = make_pair(it->first, (it->second).cnt);
            }
            else if(temp == min) //变异距离相当的时候,比较词频
            {
                if(only->second < (it))
            }
    }




    //cout << word;
    /*
    map<string, Fre_line>::iterator it = word_.find(word);
    if(it != word_.end())
    {
        ostringstream os;
        os << s << " occurs " << (it->second).cnt << " with a given key" << endl; 
        ret = os.str();
    }
    else
    {
        ret = "Not search this word !\n" ;
    }
    */
    return ret;
}

int TextQuery::edit_distance(const string &s1, const string &s2)
{
    memset(memo_, 0, sizeof(memo_));
    int m = s1.size();
    int n = s1.size();
    for(int i = 1; i <= m; ++i)
        memo_[i][0] = i;
    for(int j = 1; j <= n; ++j)
        memo_[0][j] = j;
    for(int i = 1; i <= m; ++i)
    {
        for(int j = 1; j <= n; ++j)
        {
            int inserts = memo_[i][j-1] + 1;
            int deletes = memo_[i-1][j] + 1;
            int replaces;
            if(s1[i-1] == s1[j-1])
                replaces = memo_[i-1][j-1];
            else
                replaces = memo_[i-1][j-1] + 1;
            memo_[i][j] = min(min(inserts, deletes), replaces);
        }
    }
    return memo_[m][n];
}


int cmp(const pair<string, int> &a, const pair<string, int> &b)
{
    return a.second > b.second;
}

void TextQuery::sort_map(int size)
{
    vector< pair<string, int> > vec;
    //将单词和词频放入一个vector<pair>
    for(map<string,Fre_line>::iterator it = word_.begin(); it != word_.end(); ++it)
    {
        vec.push_back(make_pair(string(it->first), (it->second).cnt));
    }
    //vector 排序
    sort(vec.begin(), vec.end(), cmp);
    for(int i = 0; i != size; ++i)
    {
        cout << vec[i].first << " " << vec[i].second << endl;    
    }
}

/*
int main(int argc, const char *argv[])
{
    string s;
    string ret;
    TextQuery text;
    text.open_file();
    text.read_file();
    text.make_map();
    while(cin >> s)
    {
            ret = text.search_file(s);
            cout << ret << endl;
        }
   // int size;
   //cin >> size;
            text.sort_map(10);
            return 0;
                }
                */
