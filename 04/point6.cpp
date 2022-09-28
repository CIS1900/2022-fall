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

    point operator+(const point & p) const
    {
        return point {x + p.x, y + p.y};
    }
};

int main()
{
    point p {1, -1};
    p = p + p;

    std::cout << "distance from origin: " << p.distanceFromOrigin() << "\n";
}
