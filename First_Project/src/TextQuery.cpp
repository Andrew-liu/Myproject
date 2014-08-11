#include "TextQuery.h"
#include <string.h>
#include <iomanip>
using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

TextQuery::TextQuery()
    :cache_(100)
{
    memset(&key_, 0, sizeof(key_));
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

void TextQuery::write_file(string filename)
{
    ofstream out;
    out.close();
    out.clear();
    out.open(filename.c_str(), ios::in);
    if(!out)
    {
        ERR_EXIT("out.open");
    }

    for(map<string, Fre_line>::iterator it = word_.begin(); it != word_.end(); ++it)
    {
        out  << left << setw(20) << it->first << setw(10) << (it->second).cnt << endl;
    }
    out.close();   
}

void TextQuery::make_index()
{
    for(map<string, Fre_line>::iterator it = word_.begin(); it != word_.end(); ++it)
    {
        pair<string, int> temp = make_pair(it->first, (it->second).cnt); //单词,词频
        insert_index(temp);
    }
}

void TextQuery::insert_index(std::pair<string, int> word)
{
    for(string::iterator it = word.first.begin(); it != word.first.end(); ++it)
    {
        index_[*it - 97].insert(word);//小写字母-97就是map对象字母表的下表
    }
}

void TextQuery::make_queue(string &s)
{ 
    int temp;
    set<candidate> set_words;
    while(!cand_.empty())
    {
        cand_.pop();
    }
    string word(s.begin(), s.end());
    cout << word << " ---queue" <<  endl;
    for(string::iterator ix = word.begin(); ix != word.end(); ++ix)
    {
        if(*ix == 'i' || *ix == 'e' || *ix == 'a')
            continue;
        for(map<string, int>::iterator iter = index_[*ix - 97].begin(); iter != index_[*ix - 97].end(); ++iter )
        {
            temp = edit_distance(word, iter->first);
            //cout << temp << endl;
            if(temp >= 3)
                continue;
            if(temp < 3)
            {
                candidate cand;
// bug     memset(&cand, 0, sizeof(cand));
                cand.word = iter->first;
                cand.distance = temp;
                cand.frequence = iter->second;
                //cout << cand.word << " " << cand.frequence << " " << cand.distance << endl;
                //
                //首先放入set集合,防止重复,再写入cand_
                set_words.insert(cand);
             }
            
        }
    }
    //如果集合为空的时候进行优先队列查询
    if(set_words.empty())
    {

        for(map<string, Fre_line>::iterator it = word_.begin(); it != word_.end(); ++it)
        {
            temp = edit_distance(word, it->first);
            //cout << temp << endl;
            if(temp >= 3)
                continue;
            if(temp < 3)
            {
                candidate cand;
// bug     memset(&cand, 0, sizeof(cand));
                cand.word = it->first;
                cand.distance = temp;
                cand.frequence = (it->second).cnt;
//              cout << cand.word << " " << cand.frequence << " " << cand.distance << endl;
                cand_.push(cand);
            }
        }
    }
    else
    {
        for(set<candidate>::iterator im = set_words.begin(); im != set_words.end(); ++im)
        {
            cand_.push(*im);
        }
    }
}
string TextQuery::return_key()
{
    string ret, temp;
    int over;
    if(cand_.size() < 3)
    {
        over = cand_.size();
    }
    else
    {
        over = 3;
    }
    for(int i = 0; i != over; ++i)
    {
        temp = cand_.top().word; 
        cout << temp << endl;
        ret += temp + " ";
        cand_.pop();
    }
    ret += "\r\n";
    return ret;
}

string TextQuery::search_cache(string &s)
{
    string word(s.begin(), ----s.end());
    cout << word << " --yes" << endl;
    string ret(cache_.get(word));
    if(ret == string())
    {      
//        ret = search_file(word);
        make_queue(word);
        ret = return_key();
        cache_.put(word, ret);
    }
    return ret;
}

void TextQuery::write_cache()
{
    ofstream in;
    in.close();
    in.clear();
    in.open(config_.return_wcache(), ios::in);
    if(!in)
    {
        ERR_EXIT("write cache!");
    }
    //访问cache进行遍历 
    vector< pair<string, string> > temperory(cache_.find_all());
    for(vector< pair<string, string> >::iterator it = temperory.begin(); it != temperory.end(); ++it)
    {
        in << left << setw(20)<< it->first << left << setw(20) << it->second << endl;
    }
    in.close();
}

string TextQuery::search_file(string &s)
{
    pair<string, int> only;
    string word(s.begin(), ----s.end());
    int min = 4, temp;
    for(map<string, Fre_line>::iterator it = word_.begin(); it != word_.end(); ++it)
    {
        temp = edit_distance(word, it->first);
        if(temp > 3)
            continue;
       
            if(temp < min)//编辑距离小于最小值的时候, 保存单词和词频
            {
                min = temp;
                cout << it->first << endl;
                only = make_pair(it->first, (it->second).cnt);
            }
            else if(temp == min) //编辑距离相当的时候,比较词频
            {
                if(only.second > (it->second).cnt)
                {
                    only = make_pair(it->first, (it->second).cnt);
                }
                else //词频相等或者大于的时候直接进入下一次循环
                    continue;
            }
    }
    if(min == 4)
    {
        string ret("No match word!!\n");
        return  ret;
    }
    else
    {
        return only.first;
    }
}

int TextQuery::edit_distance(const string &s1, const string &s2)
{
    memset(memo_, 0, sizeof(memo_));
    int x = s1.size();
    int y = s2.size();
    //初始化
    for(int i = 1; i <= x; ++i)
    {
        memo_[i][0] = i;
    }
    for(int j = 1; j <= y; ++j)
    {
        memo_[0][j] = j;
    }
    for(int i = 1; i <= x; ++i)
    {
        for(int j = 1; j <= y; ++j)
        {
            int inserts = memo_[i][j-1] + 1;
            int deletes = memo_[i-1][j] + 1;
            int replace;
            if(s1[i-1] == s2[j-1])
                replace = memo_[i-1][j-1];
            else
                replace = memo_[i-1][j-1] + 1;
            memo_[i][j] = min(min(inserts, deletes), replace);
        }
    }
    return memo_[x][y];
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
