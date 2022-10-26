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
    // The loops are to make the deadlock more common, since it's rare that the two mutex locks happen in the right sequence to trigger the deadlock.
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l1 {m1};
        scoped_lock l2 {m2};

        i1 = i;
        i2 = i;

        cerr << "f";
    }
}

void g(int i)
{
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l2 {m2};
        scoped_lock l1 {m1};

        i1 = i;
        i2 = i;

        cerr << "g";
    }
}

int main()
{
    jthread t1 {f, 1};
    jthread t2 {g, 3};
}
