#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <chrono>

using namespace std;

queue<int> q;
condition_variable cond;
mutex m;

void consume()
{
    while (true)
    {
        unique_lock lock {m};
        while (q.empty())
            cond.wait(lock);

        cerr << this_thread::get_id() << ": " << q.front() << "\n";
        q.pop();
    }
}

int main()
{
    thread c1 {consume};

    // We can add more consumers
    // thread c2 {consume};
    // thread c3 {consume};

    for (int i = 0; ; ++i)
    {
        {
            scoped_lock lock {m};
            q.push(i);
        }
        this_thread::sleep_for(1s);

        // cond.notify_one();
        cond.notify_all();
    }
}
