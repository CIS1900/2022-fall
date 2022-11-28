#include "shapesoln.hpp"

using namespace std;

shape::shape(int l, function<double(int)> f) :
    sideLength {l},
    f {f}
{}

void shape::setSideLength(int l)
{
    sideLength = l;
}

double shape::getArea()
{
    return f(sideLength);
}
