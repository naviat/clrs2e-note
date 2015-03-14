#include <stdlib.h>

/*
 * Generate a uniform random permutation of array A.
 */
void randomize_in_place(int *A, unsigned int n)
{
    unsigned int i, j;
    int temp;

    for (i = 0; i < n; i++) {
        j = i + rand() % (n - i);
        temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}
