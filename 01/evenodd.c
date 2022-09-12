#include <stdio.h>

int even(int i)
{
    if (i == 0)
        return 1;

    return odd(i - 1);
}

int odd(int i)
{
    if (i == 0)
        return 0;

    return even(i - 1);
}

int main()
{
    printf("Is 9 even: %d\n", even(9));
}
