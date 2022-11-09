#include <concepts>
#include <iostream>

using namespace std;

class foo{};

template<typename T>
concept addable = requires (const T & x) { x + x; }; // requires x + x to be valid

template<addable T> // could also use requires
T add(const T & a, const T & b)
{
    return a + b;
}

int main()
{
    cout << add(1, 2);

    // Uncomment to see the error
    // cout << add(foo(), foo());
}
