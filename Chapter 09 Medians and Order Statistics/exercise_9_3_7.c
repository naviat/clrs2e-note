/*
 * exercise_9_3_7.c
 *
 * Implement an O(n)-time algorithm that, given a set S of n distinct numbers and
 * a positive integer k ≤ n, determines the k numbers in S that are closest to
 * the median of S.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int elem_t;

extern elem_t
select_order_statistic(
       	elem_t *const A,
       	const unsigned int i,
       	const unsigned int size);

/*
 * find_k_closest
 *
 * Find the k numbers in A that are closest to the median of A.
 *
 * @param n          size of A
 * @param closest    the array storing the resulted k numbers
 */
void find_k_closest(
	elem_t *const A,
	const unsigned int n,
	elem_t *const closest,
	const unsigned int k)
{
    unsigned int i, j;
    const unsigned int median = (n + 1) / 2 - 1;
    const unsigned int head = median  - k / 2;
    const unsigned int tail = median + k - k / 2;
    select_order_statistic(A, median, n);
    select_order_statistic(A, head, median);
    select_order_statistic(A + median + 1, k - k / 2, n - median - 1);
    for (i = head, j = 0; i < median; i++, j++)
	closest[j] = A[i];
    for (++i; i <= tail; i++, j++)
	closest[j] = A[i];
}

#define MAX_INCRE 10
#define MAX_SIZE  30

int main()
{
    elem_t *A, *B;
    elem_t *closest;
    unsigned int i, k, n;

    srand(time(NULL));

    n = (unsigned int) rand() % MAX_SIZE + 1;
    A = (elem_t *) malloc(n * sizeof(elem_t));
    B = (elem_t *) malloc(n * sizeof(elem_t));    /* backup array, used for checking the algorithm */

    /* Build an random array with n distinct numbers. */
    A[0] = B[0] = rand() % MAX_INCRE;
    for (i = 1; i < n; i++)
	A[i] = B[i] = A[i - 1] + rand() % MAX_INCRE;

    /* random suffle */
    for (i = 0; i < n; i++) {
	int j = rand() % (n - i) + i;
	int tmp = A[i];
	A[i] = A[j];
	A[j] = tmp;
    }

    /* Random array building completed. */

    printf("The original array A:\n");
    i = 0;
    do {
	printf("[%u] %d\t", i, A[i]);
	++i;
	if (i % 7 == 0 && i < n)
	    putchar('\n');
    } while(i < n);
    printf("\n\n");

    k = (unsigned int) rand() % (n - 1) + 1;
    closest = (elem_t *) malloc(k * sizeof(elem_t));

    /* Invoke the tested function. */
    find_k_closest(A, n, closest, k);

    /* Print the result. */
    printf("The %u numbers that are closest to the median [%u] %d is: ",
	    k, (n + 1) / 2 - 1, A[ (n + 1) / 2 - 1 ]);
    for (i = 0; i < k - 1; i++)
	printf("%d, ", closest[i]);
    printf("%d\n\n", closest[k - 1]);

    printf("After sorting, A becomes:\n");
    i = 0;
    do {
	printf("[%d] %d\t", i, B[i]);
	++i;
	if (i % 7 == 0 && i < n)
	    putchar('\n');
    } while(i < n);
    putchar('\n');

    free(closest);
    free(B);
    free(A);

    return 0;
}

