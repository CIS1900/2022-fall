#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l {1, 2, 2, 4};

    for (auto it {l.begin()}; it != l.end(); )
    {
        if (*it == 2)
        {
            // erase returns an iterator to the next element past the erased element
            it = l.erase(it);

            // l.erase(it); // `it` is invalidated here!
        }
        else
        {
            ++it;
        }
    }

    for (int i : l)
        cout << i << "\n";
}
