#include "shape.hpp"

using namespace std;

shape::shape(int l) : sideLength {l}
{}

void shape::setSideLength(int l)
{
    sideLength = l;
}

double shape::getArea()
{
    return 0;
}
