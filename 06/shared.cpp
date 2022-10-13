#include <iostream>
#include <memory>

using namespace std;

class foo
{
public:
    void test()
    {
        cerr << "still alive\n";
    }

    ~foo()
    {
        cerr << "~foo\n";
    }
};

int main()
{
    {
        auto p {make_shared<foo>()};
        auto q {p};
        // the reference count is at 2

        p.reset();
        // the reference count is at 1 now
    }
    // now q is destroyed, since it goes out of scope

	cerr << "both shared_ptrs released\n";
}
