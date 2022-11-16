#include <iostream>
#include <concepts>

using namespace std;

template<typename T, predicate<T> P>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    int limit = 10;

    auto comparison = [&](int i)
                      {
                          return i < limit;
                      };

    cout << algo(comparison, 11) << " ";
    limit = 9; // This will only affect comparison if we captured by reference
    cout << algo(comparison, 9) << "\n";
}
