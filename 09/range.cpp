#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;

    for (int i {0}; i < 10; ++i)
        v.emplace_back(i);

    cout << v[0] << "\n";
    cout << v[10] << "\n"; // undefined behavior

    try
    {
        cout << v.at(10) << "\n";
    }
    catch (const out_of_range &)
    {
        cout << "Out of range access\n";
    }
}
