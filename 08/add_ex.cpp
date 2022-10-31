#include <iostream>
#include <thread>
#include <queue>

using namespace std;

queue<int> q;


void producer(int initial)
{
    for (int i {initial};; ++i)
    {
        this_thread::sleep_for(1s);

        q.push(i);
    }
}

void adder()
{
    while (true)
    {
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

    jthread c1 {adder};
    jthread c2 {adder};
}
