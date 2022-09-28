#include <iostream>
#include <cmath>

class point
{
    int x;
    int y;

public:
    // in the initializer list C++ can differentiate between the parameter and the member variable, but in the body of the constructor you would have to use this->x to refer to the member variable rather than the parameter
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
