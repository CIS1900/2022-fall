#include <iostream>
#include <cmath>

class point
{
    int x = 0;
    int y = 0;

public:
    point(int x, int y) : x {x}, y {y}
    {}

    double distanceFromOrigin() const
    {
        return std::sqrt(x * x + y * y);
    }
};

int main()
{
    const point p {1, -1};

    // would not work if distanceFromOrigin was not marked as const
    std::cout << "distance from origin: " << p.distanceFromOrigin() << "\n";
}
