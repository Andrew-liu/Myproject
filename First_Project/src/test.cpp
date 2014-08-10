#include "Cache.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main(int argc, const char *argv[])
{
    Cache<const char *, string> c(100);
    string s = "hello";
    c.put(s.c_str(), "hell");
    cout << c.get(s.c_str()) << endl;
    return 0;
}
