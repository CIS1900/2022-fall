#include <iostream>
#include <vector>

using namespace std;

// Try using objects of this class as the contents of the vector. These operations are necessary to do the corresponding checks.
class test
{
public:
    bool operator==(const test & t) const
    {
        return true;
    }

    bool operator<(const test & t) const
    {
        return true;
    }
};

int main()
{
    vector<int> v1 {1, 2, 2};
    vector<int> v2 {1, 2, 3};

    cout << (v1 == v2) << "\n";
    cout << (v1 < v2) << "\n";

    swap(v1, v2);
    for (int i : v1)
        cout << i << "\n";

    v1 = move(v2);
    for (int i : v1)
        cout << i << "\n";
}
