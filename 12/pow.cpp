#include <iostream>

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
    cout << pow<2>(10) << "\n";
}
