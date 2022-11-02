#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};
    vector<int> v {1, 2, 3, 4};

    // cout << *(l.begin() + 2) << "\n"; // lists do not provide random access iterators. You could use advance(it, 2) but it will result in 2 operations.
    cout << *(v.begin() + 2) << "\n";
}
