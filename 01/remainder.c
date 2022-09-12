#include <stdio.h>

void div(const int a, const int b, int * q, int * r)
{
    if (b == 0)
        return;

    *q = a / b;
    *r = a % b;
}

int main()
{
    int q = 0;
    int r = 0;

    div(9, 2, &q, &r);

    printf("9 / 2 = %d remainder %d\n", q, r);
}
