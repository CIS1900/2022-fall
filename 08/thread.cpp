#include <thread>
#include <iostream>

using namespace std;

int f(int i, int & result)
{
    cerr << i << i + 1;

    // the return value is not usable in main, use the result reference to return things to the main thread
    return i + 1;
}

int main()
{
    int i1;
    int i2;

    thread t1 {f, 1, ref(i1)};
    thread t2 {f, 3, ref(i2)};

    t1.join();
    t2.join();
}
