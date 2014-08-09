#include "Framework.h"

using namespace std;

int main(int argc, const char *argv[])
{
    Framework frame(InetAddress(8888));
    frame.start();
    return 0;
}
