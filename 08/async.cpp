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
    // By default, async is allowed to do either async or deferred
    auto result = async(f, 12);

    // You can also specify explicitly
    // auto result = async(launch::async, f, 12);
    // auto result = async(launch::deferred, f, 12);

    this_thread::sleep_for(1s);

    cout << "waiting for result\n";

    cout << result.get();
}
