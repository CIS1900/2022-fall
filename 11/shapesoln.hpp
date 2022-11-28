#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <functional>

class shape
{
private:
    int sideLength = 0;
    std::function<double(int)> f;

public:
    shape(int, std::function<double(int)>);

    void setSideLength(int);
    double getArea();
};

#endif
