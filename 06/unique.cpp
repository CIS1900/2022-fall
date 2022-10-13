#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

class foo
{
public:
    void test()
    {
        cerr << "Can use member functions normally\n";
    }

    ~foo()
    {
        cerr << "~foo\n";
    }
};

unique_ptr<foo> f(unique_ptr<foo> p)
{
    return p; // works since returning a local variable can be optimized to a move
}

int main()
{
    // two ways of initializing a unique_ptr
    unique_ptr<foo> p {new foo};
    auto q {make_unique<foo>()};

    // p = q; // doesn't work, since copy assignment is disabled
    p = move(q);

    q = make_unique<foo>();
    // p = f(q); // doesn't work
    p = f(move(q));

    p->test();

    assert(!q); // q is invalidated after the move
}
