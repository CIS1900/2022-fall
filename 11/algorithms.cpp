#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

int main()
{
    // vector of size 100
    vector<int> v(100);

    iota(v.begin(), v.end(), 0);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    random_device rd;
    default_random_engine g{rd()};

    shuffle(v.begin(), v.end(), g);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    replace_if(v.begin(), v.end(), [](auto & i)
                                   {
                                       return i > 50;
                                   }, 50);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    sort(v.begin(), v.end());

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";
}
