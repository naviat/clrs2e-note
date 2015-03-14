/*
 * test_select.c
 *
 * Testing the SELECT algorithm described in Section 9.3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Comparison function used by qsort()
 */
static int compar(const void *e1, const void *e2)
{
    return *((int *) e1) - *((int *) e2);
}

#define MAX_ELEM 99
#define MAX_SIZE 50

int main()
{
    int *A, *B;
    int i;
    int size;

    /* Before the SELECT algorithm. */

    srand(time(NULL));

    size = rand() % MAX_SIZE + 1;
    A = (int *) malloc(size * sizeof(int));
    B = (int *) malloc(size * sizeof(int));

    printf("The original array A:\n");
    i = 0;
    do {
	A[i] = B[i] = rand() % (MAX_ELEM + 1);
	printf("[%2d] %2d\t", i, A[i]);
	++i;
	if (i % 7 == 0 && i < size)
	    putchar('\n');
    } while(i < size);
    printf("\n\n");

    /* Invoke the SELECT algorithm. */

    i = rand() % size;
    printf("The %dth order statistic is: %d\n\n", i,
	    select_order_statistic(
		A, (unsigned int) i, (unsigned int) size));

    /* SELECT completed. */

    /* Sort the original array for checking. */
    qsort(B, (size_t) size, sizeof(int), compar);

    printf("After sorting, A becomes:\n");
    i = 0;
    do {
	printf("[%2d] %2d\t", i, B[i]);
	++i;
	if (i % 7 == 0 && i < size)
	    putchar('\n');
    } while(i < size);
    putchar('\n');

    free(B);
    free(A);

    return 0;
}

