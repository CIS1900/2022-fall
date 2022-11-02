#include <iostream>
#include <array>

using namespace std;

int main()
{
    int bad[] {1, 2, 3};

    // C-style arrays are prone to subtle bugs like this (undefined behavior)
    for (int i = 0; i <= 3; ++i)
        cout << bad[i] << "\n";

    array arr{1, 2, 3}; // the full type is array<int, 3>, but those arguments can be deduced.
    for (unsigned int i {0}; i < arr.size(); ++i)
        cout << arr[i] << "\n";

    // Bound-checked access is also available
    try
    {
        for (unsigned int i = 0; i <= 3; ++i)
            cout << arr.at(i) << "\n";
    }
    catch (...)
    {
        cout << "Caught exception\n";
    }

    // We can also access elements like this, since the size is fixed.
    // This check is at *compile* time!
    cout << get<0>(arr) << "\n";

    // cout << get<3>(arr) << "\n"; // Won't compile
}
