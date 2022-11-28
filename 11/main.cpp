#include "shapesoln.hpp"
#include <iostream>

using namespace std;

int main()
{
    // square
    shape s {2, [](int i){return i * i;}};

    cout << s.getArea();
}
