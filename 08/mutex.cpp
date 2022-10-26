#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;
int n = 0;

void f()
{
    for (int i {0}; i < 1'000'000; ++i) // these number separators are useful for counting the digits
    {
        // without the mutex, one thread may read n then the other thread can change it before the write goes through, resulting in a data race
        m.lock();
        n += 1;
        m.unlock();
    }
}

int main()
{
    thread t1 {f};
    thread t2 {f};

    t1.join();
    t2.join();

    cout << n;
}
