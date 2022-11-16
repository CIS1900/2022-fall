#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};
    auto end {remove(v.begin(), v.end(), 1)};

    // using the newly returned end iterator
    for_each(v.begin(), end, [](auto & i)
                             {
                                 cout << i << " ";
                             });
    cout << "\n";

    // the original container
    for (auto & i : v)
        cout << i << " ";
}
