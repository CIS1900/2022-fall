#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m1; // protects i1
mutex m2; // protects i2

int i1;
int i2;

void f(int i)
{
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l {m1, m2};

        i1 = i;
        i2 = i;

        cerr << "f";
    }
}

void g(int i)
{
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l {m2, m1};

        i1 = i;
        i2 = i;

        cerr << "g";
    }
}

int main()
{
    thread t1 {f, 1};
    thread t2 {g, 3};

    t1.join();
    t2.join();
}
