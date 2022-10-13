#include <iostream>
#include <memory>

using namespace std;

class test
{
    int * p;
    int * q;

public:
    test()
        try : p {new int}
        , q {new int [10000000000000000]}
    {}
    catch (...)
    {
        delete p;

        cerr << "Caught in ctor\n";
    }

    ~test()
    {
        delete p;
        delete [] q;
        cerr << "dtor\n"; // never called since object was never successfully constructed
    }
};

int main()
{
    try
    {
        test t;
    }
    catch (...)
    {
        cerr << "Caught in main\n";
    }
}
