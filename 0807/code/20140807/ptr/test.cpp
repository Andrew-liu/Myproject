#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Test
{
    public:
        Test() { cout << "Test" << endl;}
        ~Test() { cout << "~Test" << endl; }
};

int main(int argc, const char *argv[])
{
    unique_ptr<Test> ptr(new Test);

    return 0;
}
