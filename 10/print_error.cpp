#include <iostream>
#include <vector>
#include <list>
#include <ranges>

using namespace std;

template <typename Container>
requires ranges::range<Container>
void print(Container c)
{
    for(auto i : c)
        cout << i << "\n";
}

int main()
{
    list<int> l {1, 2, 3};
    vector<string> v {"a", "bc", ""};

    print(l);
    print(v);

    // erroneous call where the template argument is int
    print(1);
}
