#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int f(int i)
{
    throw 1;
}

int main()
{
    auto result = async(f, 12);

    try
    {
        cout << result.get();
    }
    catch (int i)
    {
        cout << "Caught " << i;
    }
}
