#include <iostream>

using std::cout;

void f(int i = 10)
{
    cout << i << "\n";
}

void g(int i, int j = 2)
{
    cout << i << " " << j << "\n";
}

int main()
{
    f();
    f(11);

    g(1);
    g(1, 3);
}
