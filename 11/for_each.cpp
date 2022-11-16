#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};

    for_each(v.begin(), v.end(), [](auto & i)
                                 {
                                     i++;
                                 });

    for (auto & i : v)
        cout << i << " ";
}
