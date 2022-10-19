#include <iostream>
#include <variant>

using namespace std;

variant<int, double> f(int choice)
{
    if (choice == 0)
        return 0.5;
    return choice;
}

int main()
{
    auto i {f(1)};

    if (holds_alternative<int>(i))
    {
        cout << "int: " << get<int>(i) << "\n";
    }
    else if (holds_alternative<double>(i))
    {
        cout << "double: " << get<1>(i) << "\n"; // can also use indices in get
    }
}
