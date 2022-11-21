#ifndef __SHAPE_H__
#define __SHAPE_H__

class shape
{
private:
    int sideLength = 0;

public:
    shape(int);

    void setSideLength(int);
    double getArea();
};

#endif
