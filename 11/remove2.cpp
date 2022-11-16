#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};
    auto end = remove(v.begin(), v.end(), 1);

    v.erase(end, v.end());

    for (auto & i : v)
        cout << i << " ";
}
