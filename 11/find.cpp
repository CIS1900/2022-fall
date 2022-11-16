#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};

    if (auto result = find(v.begin(), v.end(), 4); result != v.end())
    {
        cout << "4 found\n";
    }
    if (auto result = find(v.begin(), v.end(), 3); result != v.end())
    {
        cout << "3 found\n";
    }

    if (auto result = find_if(v.begin(), v.end(), [](const auto & i)
                                                  {
                                                      return i > 1;
                                                  }); result != v.end())
    {
        cout << "something >1 found: " << *result << "\n";
    }
}
