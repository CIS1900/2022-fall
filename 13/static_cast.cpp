#include <iostream>

using namespace std;

int main()
{
    cout << static_cast<int>(1.9) << "\n";

    // cout << static_cast<int>("asdf"s) << "\n"; // fails to compile
}
