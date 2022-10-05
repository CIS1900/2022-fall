#include <iostream>
using namespace std;

class test
{
public:
    int a;
    int b;

    // compiler warning about order
    test() : b {1}, a {b}
    {}

};

int main()
{
    test t;
    cout << t.a;
}
