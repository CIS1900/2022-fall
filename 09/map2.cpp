#include <iostream>
#include <map>

using namespace std;

class test
{
public:
    int i;

    test(int i) : i {i} {}

    bool operator<(const test & o) const
    {
        // This implementation is incorrect! If this was for sorting, it wouldn't matter.
        return i <= o.i;
    }

    bool operator==(const test & o) const
    {
        return i == o.i; // never used by map
    }
};

int main()
{
    map<test, string> m;

    test t1 {1};
    test t2 {2};
    test t3 {3};

    m[t1] = "one";
    m[t2] = "two";
    m[t3] = "three";
    m[4] = "four"; // implicit conversion also works

    // Since > was implemented incorrectly, the comparison doesn't give us the proper equality test.
    m[4] = "four, again";

    for (auto & p : m)
    {
        cout << p.first.i << " " << p.second << "\n";
    }
}
