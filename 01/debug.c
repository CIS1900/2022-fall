#include <stddef.h>

void crash(int depth)
{
    int * p = NULL;

    if (depth == 10)
        depth = *p;
    else
        crash(depth + 1);
}

int main()
{
    int a = 0;

    for (int i = 0; i < 10; ++i)
        a++;

    crash(0);
}
