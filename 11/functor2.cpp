#include <iostream>
#include <string>
#include <concepts>

using namespace std;

template <typename T>
class less_than
{
    T limit;
public:
    less_than(T limit) : limit {limit} {}

    bool operator()(T i) { return i < limit; }
};

template<typename T, predicate<T> P>
bool algo(P p, T i)
{
    return p(i);
}

bool foo(int i)
{
    return i == 1;
}

int main()
{
    less_than c1 {10};
    // if you specify the types, it would be <int, less_than<int>>
    cout << algo(c1, 11) << " " << algo(c1, 9) << "\n";

    less_than c2 {"c"s}; // the s makes it a string rather than a char[]
    // the deduced types here would be <string, less_than<string>>
    cout << algo(c2, "d"s) << " " << algo(c2, "a"s) << "\n";

    // the deduced types here would be <int, bool(int)>
    cout << algo(foo, 11) << " " << algo(foo, 1) << "\n";
}
