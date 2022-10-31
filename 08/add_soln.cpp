#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

queue<int> q;

mutex m;
condition_variable cv;

void producer(int initial)
{
    for (int i {initial};; ++i)
    {
        this_thread::sleep_for(1s);

        q.push(i);

        if (q.size() >= 2)
            cv.notify_all();
    }
}

void adder()
{
    while (true)
    {
        unique_lock l {m};

        while (q.size() < 2)
            cv.wait(l);

        int a {q.front()};
        q.pop();
        int b {q.front()};
        q.pop();

        cerr << a + b << "\n";
    }
}

int main()
{
    jthread p1 {producer, 0};
    jthread p2 {producer, 2};
    jthread p3 {producer, 2};
    jthread p4 {producer, 2};
    jthread p5 {producer, 2};

    jthread c1 {adder};
    jthread c2 {adder};
}
