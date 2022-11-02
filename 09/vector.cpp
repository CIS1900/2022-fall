#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;

    for (int i {0}; i < 10; ++i)
        v.push_back(i);

    for (unsigned int i {0}; i < v.size(); ++i)
        cout << v[i] << "\n";

    for (auto i : v)
        cout << i << "\n";
}
