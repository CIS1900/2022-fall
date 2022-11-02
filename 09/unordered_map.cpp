#include <iostream>
#include <unordered_map>

using namespace std;

int main()
{
    unordered_map<int, string> numbers;
    numbers[0] = "zero";
    numbers[1] = "one";

    unordered_map<int, string> numbers2 {{2, "two"}, {3, "three"}};

    cout << numbers[0] << " " << numbers[1] << "\n";
    cout << numbers[2]; // creates the element if it doesn't exist

    cout << numbers.size() << "\n";

    for (auto & i : numbers)
        cout << i.first << ": " << i.second << "\n";
}
