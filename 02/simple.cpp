#include <iostream>

// increments the value pointed to by p by n
void incr(int & p, int n)
{
    p += n;
}

int main(void)
{
    // could and should be a normal stack variable, left as a pointer for illustrative purposes
    int * total {new int {0}};

    for (int i = 0; i < 10; ++i)
    {
        incr(*total, i);

        std::cout << *total << " ";
    }

    std::cout << "\ntotal: " << *total << "\n";
    delete total;
}
