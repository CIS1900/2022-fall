#include <iostream>
#include <cmath>

class point
{
    int x = 0;
    int y = 0;

public:
    point(int x, int y) : x {x}, y {y}
    {}

    double distanceFromOrigin()
    {
        return std::sqrt(x * x + y * y);
    }
};

int main()
{
    point p {1, -1};

    std::cout << "distance from origin: " << p.distanceFromOrigin() << "\n";
}
