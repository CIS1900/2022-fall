#include <iostream>

constexpr long collatz(long i)
{
    if (i == 1)
        return 0;
    if (i % 2 == 0)
        return 1 + collatz(i / 2);
    else
        return 1 + collatz(3 * i + 1);
}

int main()
{
    constexpr int n {collatz(63728127)};

    int i;
    std::cin >> i;
    std::cout << collatz(i);
}
