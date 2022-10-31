#include <thread>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <utility>

using namespace std;

shared_mutex sm;

pair<int, int> state;

void write()
{
    for (int i {0}; ; ++i)
    {
        this_thread::sleep_for(1s);

        unique_lock lock {sm};
        state.first = i;
        state.second = i + 1;
    }
}

void read()
{
    while (true)
    {
        this_thread::sleep_for(1s);

        shared_lock lock {sm};
        cerr << this_thread::get_id() << ": " << state.first << "," << state.second << "\n";
    }
}

int main()
{
    jthread w1 {write};
	jthread w2 {write};

    jthread r1 {read};
    jthread r2 {read};
    jthread r3 {read};
}
