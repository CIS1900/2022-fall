#include <iostream>
#include <vector>
#include <functional>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 4};

    using namespace placeholders;

    // bind gives us the function (_ < 3), and replaces everything < 3 with 3.
    replace_if(v.begin(), v.end(), bind(less<int>(), _1, 3), 3);

    for (auto & i : v)
        cout << i << " ";
}
