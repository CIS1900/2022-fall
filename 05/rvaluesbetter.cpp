#include <iostream>

using namespace std;

int global = 0;

int & f()
{
    return global;
}

void test(const int & i)
{
    cout << "lvalue\n";
}

// Try commenting out this overload of test. Even if you call with an rvalue, it is able to fall back to a const lvalue reference overload, just like with a copy constructor.
void test(int && i)
{
    cout << "rvalue\n";
}

int main()
{
    // the return value of a function can be an lvalue too, if it's returning a reference
    f() = 1;

    int i = 2;
    test(i);
    test(f());

    // two ways of calling with an rvalue
    test(move(i));
    test(3);
}
