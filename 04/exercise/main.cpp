#include "dog.hpp"
#include <iostream>

void petLots(const animal & a)
{
    for (int i {0}; i < 10; ++i)
    {
        a.pet();
        std::cout << "\n";
    }
}

int main()
{
    dog d {"Rover"};
    petLots(d);
}
