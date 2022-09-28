#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class point
{
    int x = 0;
    int y = 0;

public:
    point(int, int);
    double distanceFromOrigin() const;
    point operator+(const point &) const;

    // remember not to use using in headers, since it will force everyone who includes your header to import the namespace as well
    friend std::ostream & operator<<(std::ostream &, const point &);
    friend std::istream & operator>>(std::istream &, point &);
};

std::ostream & operator<<(std::ostream &, const point &);
std::istream & operator>>(std::istream &, point &);

#endif
