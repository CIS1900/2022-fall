constexpr long int factorial(long int i)
{
    if (i <= 1)
        return 1;

    return i * factorial(i - 1);
}

int main()
{
    constexpr long int i = factorial(12);
}
