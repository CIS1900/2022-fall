#include <iostream>

using namespace std;

int main()
{
    string name;

    cout << "Enter your name: ";
    cin >> name; // only reads up to a space
    // getline(cin, name); // to read the entire line
    cout << "Hello " << name << "!\n";
}
