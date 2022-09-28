#include <iostream>
#include "point.hpp"

int main()
{
    point p {1, -1};

    std::cout << p << "\n";

    if (std::cin >> p)
    {
        std::cout << p << "\n" << p.distanceFromOrigin() << "\n";
    }
}
