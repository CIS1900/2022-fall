#include <iostream>
#include <cmath>

class point
{
    // now x and y are private, could add
    // private:
    // but it is already implicit
    int x;
    int y;

public:
    double distanceFromOrigin()
    {
        return std::sqrt(x * x + y * y);
    }
};

int main()
{
    point p;
    // x and y are no longer accessible from outside the class
    // p.x = 1;
    // p.y = -1;

    std::cout << "distance from origin: " << p.distanceFromOrigin() << "\n";
}
