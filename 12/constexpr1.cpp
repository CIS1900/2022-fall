#include <iostream>
#include <vector>

using namespace std;

template<int base>
int pow(int exponent)
{
    int val {1};
    for (int i {0}; i < exponent; ++i)
    {
        val *= base;
    }

    return val;
}

int main()
{

    // int i;
    // cin >> i;
    // const int b {i}; // cannot be made constexpr, since i is not known at compile-time.
    constexpr int b {2};
    cout << pow<b>(10) << "\n";
}
