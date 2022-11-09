#include <vector>
#include <list>
#include <iostream>

using namespace std;

void edit(vector<int> & v)
{
    for (auto it {v.begin()}; it != v.end();)
    {
        if (*it == 1)
        {
            // goes to the next element, so no need to do ++
            it = v.erase(it);
        }
        else if (*it == 0)
        {
            it = v.insert(it, 0);
            advance(it, 2); // for a random access iterator, it just does it += 2
        }
        else
        {
            it++;
        }
    }
}

void edit(list<int> & l)
{
    for (auto it {l.begin()}; it != l.end();)
    {
        if (*it == 1)
        {
            // goes to the next element, so no need to do ++
            it = l.erase(it);
        }
        else if (*it == 0)
        {
            it = l.insert(it, 0);
            advance(it, 2); // for a bidirectional iterator, it does it++ twice
        }
        else
        {
            it++;
        }
    }
}

int main()
{
    vector<int> v {1, 2, 3, 0, 0};
    for (auto i : v)
        cout << i << " ";
    cout << endl;

    edit(v);
    for (auto i : v)
        cout << i << " ";
    cout << endl;

}
