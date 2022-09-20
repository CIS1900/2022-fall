#include <iostream>

using std::cout, std::cerr;

int main()
{
    // doesn't print due to buffering
    cout << "test";
    abort();

    // the following 3 do work, since they flush the buffer before crashing.
    // cout << "test" << endl;
    // abort();

    // cout << "test";
    // cout.flush();
    // abort();

    // cerr << "test";
    // abort();
}
