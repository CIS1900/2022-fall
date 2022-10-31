#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;
unsigned long long n {0};

void f()
{
    for (int i {0}; i < 50'000'000; ++i)
    {
        scoped_lock l {m};
        n += i;
    }
}

int main()
{
    thread t1 {f};
    f();

    t1.join();

    cout << n;
}
