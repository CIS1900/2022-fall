#include <iostream>
#include <cmath>

using std::istream, std::ostream, std::string;

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

    friend ostream & operator<<(ostream &, const point &);
    friend istream & operator>>(istream &, point &);
};

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

int main()
{
    point p {1, -1};

    std::cout << p << "\n";

    if (std::cin >> p)
    {
        std::cout << p << "\n" << p.distanceFromOrigin() << "\n";
    }
}
