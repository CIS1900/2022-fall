#include <iostream>
#include <cmath>

struct point
{
    int x;
    int y;

    double distanceFromOrigin()
    {
        return std::sqrt(x * x + y * y);
    }
};

int main()
{
    point p;
    p.x = 1;
    p.y = -1;

    std::cout << "distance from origin: " << p.distanceFromOrigin() << "\n";
}
