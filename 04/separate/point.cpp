#include "point.hpp"
#include <iostream>
#include <cmath>

using std::istream, std::ostream, std::string;

point::point(int x, int y) : x {x}, y {y}
{}

double point::distanceFromOrigin() const
{
    return std::sqrt(x * x + y * y);
}

point point::operator+(const point & p) const
{
    return point {x + p.x, y + p.y};
}

ostream & operator<<(ostream & os, const point & p)
{
    os << p.x << ", " << p.y;
    return os;
}

istream & operator>>(istream & is, point & p)
{
    string s;
    // note that this breaks if we input, e.g. 10,10 (no space after comma). Consider is.get(char &) instead for the comma.

    is >> p.x >> s >> p.y;

    if (s != ",")
        is.setstate(std::ios_base::failbit);

    return is;
}
