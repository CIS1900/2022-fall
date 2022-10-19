#include <iostream>
#include <any>

using namespace std;

any f(int choice)
{
    if (choice == 0)
        return 0.5;
    return choice;
}

int main()
{
    any a {f(1)};

    try
    {
        // throws an exception if cast fails on a regular variable
        cout << any_cast<double>(a) << '\n';
    }
    catch (const bad_any_cast& e)
    {
        cerr << e.what() << '\n';
    }

    // returns nullptr if cast fails on an any pointer
    if (int * i {any_cast<int>(&a)})
        cout << *i << "\n";
}
