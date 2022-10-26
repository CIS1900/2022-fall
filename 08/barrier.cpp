#include <barrier>
#include <iostream>
#include <thread>

using namespace std;

void done()
{
    cerr << "Barrier passed\n";
}

barrier b {5, done};

void f()
{
    b.arrive_and_wait();
}

int main()
{
    jthread f1 {f};
    jthread f2 {f};
    jthread f3 {f};
    jthread f4 {f};
    jthread f5 {f}; // if there aren't enough threads arriving at the barrier, the waiting threads will never continue and done will never be called
}
