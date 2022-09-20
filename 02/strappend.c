#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// caller is responsible for freeing memory
char * append(const char * const first, const char * const second)
{
    int l1 = strlen(first);
    int l2 = strlen(second);

    char * str = malloc((l1 + l2 + 1) * sizeof(char));

    if (!str)
        return NULL;

    for (int i = 0; i < l1; ++i)
    {
        str[i] = first[i];
    }
    for (int i = 0; i < l2; ++i)
    {
        str[i + l1] = second[i];
    }

    str[l1 + l2] = '\0';

    return str;
}

int main()
{
    char * str = append("hello", " world");
    printf("%s\n", str);

    free(str);

    return 0;
}
