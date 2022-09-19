#include <stdio.h>
#include <stdlib.h>

// increments the value pointed to by p by n
void incr(int * p, int n)
{
    *p += n;
}

int main(void)
{
    int * total = malloc(sizeof(int));
    *total = 0;

    for (int i = 0; i < 10; ++i)
    {
        incr(total, i);
        printf("%d ", *total);
    }

    printf("\ntotal: %d\n", *total);
    free(total);
}
