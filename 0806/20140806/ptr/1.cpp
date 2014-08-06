#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;


class Test{
    public:
        Test() { cout << "Test" << endl; }
        ~Test() { cout  << "~Test" << endl; }
};

int main(int argc, const char *argv[])
{
    shared_ptr<Test> ptr(new Test);

    cout << ptr.use_count() << endl;  //1
    cout << ptr.unique() << endl;  //true 1
    
    shared_ptr<Test> ptr2(ptr);  //类似于fd的复制
    cout << ptr.use_count() << endl;  //2 
    cout << ptr.unique() << endl;  // false 0

    return 0;
}









