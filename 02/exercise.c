// source: https://courses.cs.washington.edu/courses/cse303/04au/Homework/gdb/

#include <stdio.h>

int tester(int * c, int k)
{
	printf("x[%d] = %d\n", k, c[k]);
}

int main()
{
	int x[1000];

	for(int i = 0; i < 10000; ++i)
    {
		x[i] = i;
	}

	printf("Enter an integer in 0..9999: ");

    int k = 0;
	scanf("%d", k);

	tester(x, k);
}
