#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int f(int i)
{
    cout << "f called\n";

    this_thread::sleep_for(1s);

    return i + 1;
}

int main()
{
    async(f, 12);
    async(f, 12);
}
