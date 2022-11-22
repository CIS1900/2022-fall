#include <iostream>

using namespace std;

template<long int, long int>
struct CollatzHelper;

template<long int>
struct Collatz;

// A is even
template <long int A>
struct CollatzHelper<A, 0>
{
    static const long int steps = CollatzHelper<A/2, (A/2)%2>::steps + 1;
};

// A is odd
template <long int A>
struct CollatzHelper<A, 1>
{
    static const long int steps = CollatzHelper<(A*3)+1, (((A*3)+1)%2)>::steps + 1;
};

// base case
template <>
struct CollatzHelper<1, 1>
{
    static const long int steps = 0;
};

template<long int A>
struct Collatz
{
    static const long int steps = CollatzHelper<A, A%2>::steps;
};

int main() {
    cout << "Collatz stopping time of 1: "
         << Collatz<1>::steps
         << endl;
    cout << "Collatz stopping time of 27: "
         << Collatz<27>::steps
         << endl;
    cout << "Collatz stopping time of 1729: "
         << Collatz<1729>::steps
         << endl;
    cout << "Collatz stopping time of 34969: "
         << Collatz<34969>::steps
         << endl;
    cout << "Collatz stopping time of 63728127: "
         << Collatz<63728127>::steps
         << endl;
}
