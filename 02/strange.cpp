#include <iostream>

// the caller must delete this memory when they are done with it, *very* strange.
int & foo()
{
    auto * p {new int {0}};
    return *p;
}

int main()
{
    int & r {foo()};

    std::cout << r;

    delete &r;
}
