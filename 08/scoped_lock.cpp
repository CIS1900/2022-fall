#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void f(int i, int & result)
{
    try
    {
        scoped_lock lock {m};

        cerr << i << i + 1;
        throw 1;
    }
    catch (...)
    {}
}

int main()
{
    int i1;
    int i2;

    jthread t1 {f, 1, ref(i1)};
    jthread t2 {f, 3, ref(i2)};
}
