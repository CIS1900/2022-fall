#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    try
    {
        // try each of these to see different exceptions being thrown
        throw 1;
        // string {"abc"}.substr(10);
        // auto i {new int [1000000000000]};
        // throw "error message?";
    }
    catch (const int i)
    {
        cerr << i;
    }
    catch (const out_of_range & e)
    {
        cerr << "out_of_range: " << e.what();
    }
    catch (const exception e) // object slicing here
    {
        cerr << "exception: " << e.what();
    }
    catch (...)
    {
        // cannot use the actual exception here, since don't know its type
        cerr << "other\n";
        throw;
    }
}
