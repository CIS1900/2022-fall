#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2 (v1.size());

    copy(v1.begin(), v1.end(), v2.begin());

    for (auto & i : v2)
        cout << i << " ";
}
