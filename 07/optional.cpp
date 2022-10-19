#include <iostream>
#include <optional>

using namespace std;

optional<string> divide(int a, int b)
{
    if (b == 0)
    {
        return nullopt;
    }

    return to_string(a / b); // convenience function provided for most built-in types
}

int main()
{
    if (auto s {divide(10, 0)})
    {
        cout << *s << "\n";
    }

    // equivalent to above
    if (auto s {divide(10, 2)}; s.has_value())
    {
        cout << s.value() << "\n";
    }

    cout << divide(10, 0).value_or("divide by 0") << "\n";
}
