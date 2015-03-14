/*
 * Implementing formula (15.11).
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned i, j, n;
    unsigned *P;
    printf("Input a positive number n: ");
    scanf("%u", &n);

    P = (unsigned *) malloc(n * sizeof(unsigned));

    i = 0;
    P[0] = 1;
    printf("P(1) = 1\n");
    while (++i < n) {
	P[i] = 0;
	for (j = 0; j < i; j++)
	    P[i] += P[j] * P[i - j - 1];
	printf("P(%u) = %u\n", i + 1, P[i]);
    }
    
    free(P);
}

