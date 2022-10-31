#include <iostream>

using namespace std;

int main()
{
    unsigned long long n {0};

    for (int i {0}; i < 50'000'000; ++i)
    {
        n += i;
    }
    for (int i {0}; i < 50'000'000; ++i)
    {
        n += i;
    }
    cout << n;
}
