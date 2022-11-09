#include <iostream>

using namespace std;

template <typename T>
void my_swap (T & a, T & b)
{
    T c {a};
    a = move(b);
    b = move(c);
}

// equivalent to
// template <typename T, typename U>
// void my_swap_auto(T & a, U & b) ...
void my_swap_auto(auto & a, auto & b)
{
    auto c {a};
    a = move(b);
    b = move(c);
}


int main()
{
    int i {1};
    int j {2};

    my_swap(i, j); // could also do my_swap<int>(i, j);
    my_swap_auto(i, j); // or my_swap_auto<int, int>(i, j);

    cout << i << " " << j;
}
