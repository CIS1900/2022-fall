#include <vector>
#include <list>
#include <iostream>
#include <deque>
#include <array>
// #include <ranges>

using namespace std;

template <typename T>
concept range = requires (T & t)
{
    t.begin();
    t.end();
};

template <typename T>
concept editable_container = requires (T & t, T::iterator it, int n)
{
    t.insert(t.begin(), n);
    t.erase(it);
};

template <typename Container>
requires editable_container<Container> && range<Container> && same_as<typename Container::value_type, int>
void edit(Container & v)
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
            advance(it, 2);
        }
        else
        {
            it++;
        }
    }
}

int main()
{
    vector v {1, 2, 3, 0, 0};
    for (auto i : v)
        cout << i << " ";
    cout << endl;

    edit(v);
    for (auto i : v)
        cout << i << " ";
    cout << endl;

}
