#include <iostream>
#include <map>

using namespace std;

class test
{
public:
    int i;

    test(int i) : i {i} {}

    bool operator>(const test & o) const
    {
        return i > o.i;
    }
};

int main()
{
    map<test, string, greater<test>> m;

    test t1 {1};
    test t2 {2};
    test t3 {3};

    m[t1] = "one";
    m[t2] = "two";
    m[t3] = "three";
    m[4] = "four"; // implicit conversion also works

    // iterates in descending instead of ascending order
    for (auto & p : m)
    {
        cout << p.first.i << " " << p.second << "\n";
    }
}
