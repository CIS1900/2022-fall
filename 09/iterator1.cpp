#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};

    // use an iterator to update the first element
    *l.begin() = -1;

    // auto is convenient for code like this
    for (list<int>::iterator i {l.begin()}; i != l.end(); ++i)
        cout << *i << "\n";
}
