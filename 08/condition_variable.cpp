#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <optional>
#include <chrono>

using namespace std;

optional<int> value;
condition_variable cond;
mutex m;

void consume()
{
    while (true)
    {
        unique_lock lock {m};
        while (!value.has_value())
            cond.wait(lock);

        cerr << this_thread::get_id() << ": " << *value << "\n";
        value = nullopt;
    }
}

int main()
{
    thread c1 {consume};

    // We can add more consumers
    thread c2 {consume};
    thread c3 {consume};

    for (int i = 0; ; ++i)
    {
        {
            scoped_lock lock {m};
            value = i;
        }
        this_thread::sleep_for(1s);

        // cond.notify_one();
        cond.notify_all();
    }
}
