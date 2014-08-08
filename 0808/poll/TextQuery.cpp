#include "TextQuery.h"
using namespace std;
TextQuery::TextQuery(){}

TextQuery::~TextQuery(){}

void TextQuery::open_file()
{
    in.close();
    in.clear();
    in.open("The_Holy_Bible.txt");
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
    
    string ret;
    string word(s.begin(), ----s.end());
    //cout << word;
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
    return ret;
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
