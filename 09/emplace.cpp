#include <iostream>
#include <vector>

using namespace std;

class test
{
public:
    test() { cerr << "ctor\n"; }
    test(const test &) { cerr << "copy ctor\n"; }
    test(test &&) noexcept { cerr << "move ctor\n"; }
    ~test() { cerr << "dtor\n"; }
};

int main()
{
    vector<test> v;
    // v.reserve(10); // resize vector to 10 so the vector doesn't double in size as we emplace_back, which would add extra debug output. Uncomment this if you want to see only the output related to emplace.

    for (int i {0}; i < 10; ++i)
    {
        // v.push_back(test {}); // unnecessary move constructor needed
        v.emplace_back();
    }
}
