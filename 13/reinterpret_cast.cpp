#include <iostream>

using namespace std;

struct A
{
    unsigned char c[4] {1, 0, 1, 0};
};

struct B
{
    unsigned int i;
};

int main()
{
    A a;
    cout << dynamic_cast<B &>(a);
}
