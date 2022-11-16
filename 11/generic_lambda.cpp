#include <iostream>

using namespace std;

int main()
{
    auto f = [](auto a, auto b) -> decltype(a + b) // return type not necessary, can be inferred
             {
                 return a + b;
             };

    cout << f(1, 2) << "\n";
    cout << f("Hello "s, "world") << "\n";
}
