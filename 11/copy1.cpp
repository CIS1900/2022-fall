#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2;

    // undefined behavior since there's not enough space in v2
    copy(v1.begin(), v1.end(), v2.begin());
}
