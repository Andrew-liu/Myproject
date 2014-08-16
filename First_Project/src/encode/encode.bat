#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
using namespace std;
#define MAXSIZE 100
uint32_t memo_[MAXSIZE][MAXSIZE];
bool utf8toascii(const char *utf8, uint32_t *ascii)
{
    uint32_t temp1, temp2;
    uint32_t *in = (uint32_t *)utf8;
    if(0x00 == (*in & 0x80)) //0xxx xxx &1000 0000 == 0000 0000
    {//一个字节
        *ascii = (uint32_t)*utf8;
        return true;
    }
    else if(0xc0 == (*in & 0xe0) && 0x80 == (*(in + 1) & 0xc0)) // 110x xxxx 10xx xxxx &
    { //两个字节                                                   1110 0000 1100 0000
        temp1 = (uint32_t)(*in & 0x3f); //取后6位               // 1100 0000 1000 0000
        temp1 <<= 6;
        temp1 |= (uint32_t)(*(in + 1) & 0x3f);
        *ascii = temp1;
        return true;
    }
    else if(0xe0 == (*in & 0xf0) && 0x80 == (*(in + 1) &0xc0) && 0x80 == (*(in + 2) & 0xc0))
    { //三个字节   1110 xxxx  10xx xxxx  10xx xxxx  & 
        temp1 = (uint32_t)(*in & 0x0f);
        temp1 <<= 12;
        temp2 = (uint32_t)(*(in + 1) & 0x3f);
        temp2 <<= 6;
        temp1 =temp1|temp2|(uint32_t)(*(in + 2) & 0x3f);
        *ascii = temp1;
        return true;
    }
    else
    {//1111 0xxxx 10xx xxxx 10xx xxxx 10xx xxxx
        *ascii = 0x22e0;
        return false;
    }
    return false;
}

bool asciitoutf8(uint32_t *ascii, char *utf8)
{
    int length = 0;
    char *out = utf8;
    if(0x0080 > *ascii)
    {//1字节utf8编码
        *out = (char)*ascii;
        length++;
    }
    else if(0x0080 > *ascii)
    {//2字节编码
        *out = ((char)(*ascii >> 6)) | 0xc0;
        *(out + 1) = ((char)(*ascii & 0x003f))|0x80;
        length += 2;
    }
    else
    {//3字节编码
        *out = ((char)(*ascii >> 12))|0xe0;
        *(out + 1) = ((char)(*ascii & 0x0fc0) >> 6)|0x80;
        *(out + 2) = ((char)(*ascii &0x003f))|0x80;
        length += 3;
    }
    return length;
}

uint32_t edit_distance(uint32_t *s1, uint32_t *s2)
{
    memset(memo_, 0, sizeof(memo_));
    uint32_t x = sizeof(s1) / sizeof(uint32_t);
    uint32_t y = sizeof(s2) / sizeof(uint32_t);
    //初始化
    for(uint32_t i = 1; i <= x; ++i)
    {
        memo_[i][0] = i;
    }
    for(uint32_t j = 1; j <= y; ++j)
    {
        memo_[0][j] = j;
    }
    for(uint32_t i = 1; i <= x; ++i)
    {
        for(uint32_t j = 1; j <= y; ++j)
        {
            uint32_t inserts = memo_[i][j-1] + 1;
            uint32_t deletes = memo_[i-1][j] + 1;
            uint32_t replace;
            if(s1[i-1] == s2[j-1])
                replace = memo_[i-1][j-1];
            else
                replace = memo_[i-1][j-1] + 1;
            memo_[i][j] = min(min(inserts, deletes), replace);
        }
    }
    return memo_[x][y];
}

int main(int argc, const char *argv[])
{
    string s1("hell");
    string s2("hello");
    uint32_t *ascii1 = new uint32_t;
    uint32_t *ascii2 = new uint32_t;
    utf8toascii(s1.c_str(), ascii1);
    cout << ascii1 << endl;
    utf8toascii(s2.c_str(), ascii2);
    cout << ascii2 << endl;
    uint32_t ret = edit_distance(ascii1, ascii2);
    char *convert = new char;
    int length = asciitoutf8(ascii1, convert);
    cout << convert << endl;
    cout << ret << endl;
    cout << length << endl;
    return 0;
}
