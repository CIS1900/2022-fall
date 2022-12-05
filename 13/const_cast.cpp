#include <iostream>

using namespace std;

void f(const int & i)
{
    // i = 2; // fails to compile
    const_cast<int &>(i) = 2;
}

int main()
{
    const int i {1};

    f(i);

    cout << i << "\n";
}
