#include <thread>
#include <iostream>
#include <future>

using namespace std;

unsigned long long f()
{
    unsigned long long n {0};

    for (int i {0}; i < 50'000'000; ++i)
    {
        n += i;
    }
    return n;
}

int main()
{
    auto r1 {async(f)};
    auto l {f()};

    cout << r1.get() + l;
}
