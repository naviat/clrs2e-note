/*
 * An iterative version of RANDOMIZED-SELECT, also the solution to Exercise 9.2-3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 	20
#define MAX_ELEM_ABS	10 

unsigned int
randomized_partition(int *A, unsigned int p, unsigned int r)
{
    unsigned int j, k;
    int exchange;

    j = ((unsigned int) rand()) % (r - p + 1) + p;
    exchange = A[j];
    A[j] = A[r];
    A[r] = exchange;

    j = p - 1;
    for (k = p; k < r; k++)
	if (A[k] <= A[r]) {
	    ++j;
	    exchange = A[j];
	    A[j] = A[k];
	    A[k] = exchange;
	}
    
    exchange = A[++j];
    A[j] = A[r];
    A[r] = exchange;

    return j;
}

int main()
{
    int *A;
    unsigned int i, n;
    unsigned int k, p, q, r;

    /* Before the RANDOMIZED-SELECT algorithm begins */ 

    srand(time(NULL));

    n = ((unsigned int) rand()) % MAX_SIZE + 1;
    A = (int*) malloc(sizeof(int) * n);

    printf("A = {");
    for (i = 0; i < n - 1; i++) {
        A[i] = (1 - 2 * (rand() % 2)) * (rand() % (MAX_ELEM_ABS + 1));
        printf("%d, ", A[i]);
    }
    A[n - 1] = (1 - 2 * (rand() % 2)) * (rand() % (MAX_ELEM_ABS + 1));
    printf("%d}\n", A[n - 1]);

    i = ((unsigned int) rand()) % n + 1;
    printf("The No.%u smallest element in A is ", i);

    /* Now begins the RANDOMIZED-SELECT algorithm */

    --i;
    p = 0;
    r = n - 1;
    do {
	if (p == r) {
	    q = p;
	    break;
	}

	q = randomized_partition(A, p, r);
	k = q - p;

	if (i == k)
	    break;
	else if (i < k)
	    r = q - 1;
	else {
	    p = q + 1;
	    i -= k + 1;
	}
    } while(1);

    /* The algorithm ends */

    printf("%d.\n", A[q]);

    free(A);

    return 0;
}

