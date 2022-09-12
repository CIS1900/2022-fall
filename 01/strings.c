#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * f()
{
    char * s = malloc(2 * sizeof(char));
    s[0] = 'h';
    s[1] = 0;

    return s;
}

int main()
{
    char * str = f();
    printf("%s\n", str);
}
