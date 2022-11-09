#include <iostream>
#include <vector>
#include <list>

using namespace std;

template <typename Container> // could also define using auto
void print(Container c)
{
    for(auto i : c)
        cout << i << "\n";
}

int main()
{
    list<int> l {1, 2, 3};
    vector<string> v {"a", "bc", ""};

    print(l); // could be explicit and write print<list<int>>(l);
    print(v);
}
