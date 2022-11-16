#include <iostream>
#include <vector>
#include <functional>
#include <numeric>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 4};

    using namespace placeholders;

    transform(v.begin(), v.end(), v.begin(), bind(plus<int>(), 1, _1));

    for (auto & i : v)
        cout << i << " ";

    cout << "\n\n";
    cout << accumulate(v.begin(), v.end(), 0, plus<int>());
}
