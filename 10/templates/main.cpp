#include <iostream>
#include "wrapper.hpp"

using namespace std;

int main()
{
    wrapper w {12};

    cout << w.get();
}
