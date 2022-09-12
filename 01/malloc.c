#include <stdlib.h>
#include <stdio.h>

int * makeInts(int n)
{
    if (n <= 0) {
        // signal failure with NULL
        return NULL;
    }

    // the memory allocated here outlives the current scope
    int * ints = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i)
    {
        ints[i] = i;
    }

    return ints;
}

int main()
{
    int count = 0;

    scanf("%d", &count);

    int * ints = makeInts(count);
    printf("the %d-th int is %d\n", count, ints[count - 1]);

    // we must free the memory manually
    free(ints);
}
