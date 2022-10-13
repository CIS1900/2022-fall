#include <iostream>
#include <stdexcept>

using namespace std;

void f() noexcept
{
    throw 1;
}

int main()
{
    try
    {
        f();
    }
    catch (const int i)
    {
        cerr << i;
    }
}
