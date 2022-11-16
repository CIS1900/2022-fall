#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2;

    copy(v1.begin(), v1.end(), back_inserter(v2));

    for (auto & i : v2)
        cout << i << " ";
}
