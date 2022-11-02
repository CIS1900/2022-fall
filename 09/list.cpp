#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l;

    l.emplace_front(2);
    l.emplace_back(3);
    l.push_front(1);

    for (auto i : l)
        cout << i << "\n";
}
