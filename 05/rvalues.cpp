#include <iostream>

using namespace std;

ostream & f()
{
    return cout;
}

void test(ostream & os)
{
    os << "lvalue\n";
}
void test(ostream && os)
{
    os << "rvalue\n";
}

int main()
{
    f() << "test\n";

    test(cout);
    test(move(cout));
}
