#include <iostream>
#include <vector>
#include <list>
#include <ranges>

using namespace std;

template <typename Container>
requires ranges::range<Container> && requires (Container::value_type t, const Container::value_type & v) {t += v;}
auto sum(const Container & c)
{
    // need typename to tell C++ this is a type
    typename Container::value_type total {};
    for (const auto & i : c)
    {
        total += i;
    }

    return total;
}

struct foo{};

int main()
{
    vector<int> v {1, 2, 3};
    list<string> l {"h", "e", "l", "l", "o"};
    list<foo> l2 {foo(), foo()};

    cout << sum(v) << "\n";
    cout << sum(l) << "\n";
    // Uncomment for example of unsatisfied type constraint
    // cout << sum(l2);
}
