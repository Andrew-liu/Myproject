#ifndef _UTFTOSTRING_
#define _UTFTOSTRING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

namespace liubin
{
    using namespace std;
    int getLenOfUTF8(unsigned char c);
    void parseUTF8String(const string &s, vector<uint32_t> &vec);
    void parseUTF8String(const string &s, vector<uint32_t> &vec);
    int edit_distance(const string &a, const string &b);
}
    //计算UTF8编码所占的字节
    int getLenOfUTF8(unsigned char c)
    {
        int cnt = 0;
        while(c & (1 << (7-cnt)))
            ++cnt;
        return cnt; 
    }

    //e5 ae bf
    //把字符串解析成uint32_t数组
    void parseUTF8String(const string &s, vector<uint32_t> &vec)
    {
        vec.clear();
        for(string::size_type ix = 0; ix != s.size(); ++ix)
        {
            int len = getLenOfUTF8(s[ix]);
            uint32_t t = (unsigned char)s[ix]; //e5
            if(len > 1)
            {
                --len;
                //拼接剩余的字节
                while(len--)
                {
                    t = (t << 8) + (unsigned char)s[++ix];
                }
            }
            vec.push_back(t);
        }
    }

    inline int MIN(int a, int b, int c) {
        int ret = (a < b) ? a : b;
        ret = (ret < c) ? ret : c;
        return ret;
    }

    int edit_distance_uint_32(const vector<uint32_t> &w1,
            const vector<uint32_t> &w2) {
        int len_a = w1.size();
        int len_b = w2.size();
        int memo[100][100];
        memset(memo, 0x00, 100 * 100 * sizeof(int));
        for (int i = 1; i <= len_a; ++i) {
            memo[i][0] = i;
        }
        for (int j = 1; j <= len_b; ++j) {
            memo[0][j] = j;
        }
        for (int i = 1; i <= len_a; ++i) {
            for (int j = 1; j <= len_b; ++j) {
                if (w1[i - 1] == w2[j - 1]) {
                    memo[i][j] = memo[i - 1][j - 1];
                } else {
                    memo[i][j] = MIN(memo[i - 1][j - 1], memo[i][j - 1],
                            memo[i - 1][j]) + 1;
                }
            }
        }
        return memo[len_a][len_b];
    }

int edit_distance(const string &a, const string &b) {
    vector<uint32_t> w1, w2;
    parseUTF8String(a, w1);
    parseUTF8String(b, w2);
    return edit_distance_uint_32(w1, w2);
}

#endif


/*
int main(int argc, const char *argv[])
{
    
       string s = "宿青刘斌";
       vector<uint32_t> vec;
       parseUTF8String(s, vec);
       for(size_t ix = 0; ix != vec.size(); ++ix)
       {
       printf("%x\n", vec[ix]);
       } 

    string s = argv[1];
    string s2 = argv[2];
    printf("%d\n", edit_distance(s, s2));
    return 0;
}

*/
