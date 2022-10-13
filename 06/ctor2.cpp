#include <iostream>
#include <memory>

using namespace std;

class test
{
    unique_ptr<int> p;
    unique_ptr<int []> q;

public:
    test()
        try : p {make_unique<int>(0)}
        , q {make_unique<int []>(10000000000000000)}
    {}
    catch (...)
    {
        cerr << "Caught in ctor\n";
    }

    ~test()
    {
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
