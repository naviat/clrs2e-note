/*
 * exercise_9_6.c
 *
 * The $k$th quantiles of an $n$-element set are the k-1
 * order statistics that divide the sorted set into k
 * equal-sized sets (to within 1). Give an O(n \lg k)-time
 * algorithm to list the $k$th quantiles of a set.
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
 * Comparison function used by qsort()
 */
static int compar(const void *e1, const void *e2)
{
    return *((int *) e1) - *((int *) e2);
}

/*
 * find_quantiles
 *
 * Find the $k$th quantiles of an $n$-element set. The
 * algorithm takes O(n \lg k) running time.
 *
 * @param A
 * 	head address of the $n$-element set
 *
 * @param quantile
 * 	head address of the array storing the
 * 	$k$th quantiles found
 *
 * @param order
 * 	the orders in A of the quantiles
 */
void find_quantiles(
	elem_t *const A,
	const unsigned int n,
	elem_t *const quantile,
	unsigned int *const order,
	const unsigned int k)
{
    /*
     * the number of sets that are of size ⌊n/k⌋ after
     * division by the $k$th quantiles minus 1
     */
    unsigned int bigger_num;

    /*
     * the order of the order statistic we should find
     * before any recursive invocation
     */
    unsigned int pivot_order;

    unsigned int i;

    if (k <= 1)
	return;

    bigger_num = n - k * (n / k);

    if (bigger_num >= k / 2)
	pivot_order = (k / 2) * ((n + k - 1) / k) - 1;
    else
	pivot_order =
	    bigger_num * ((n + k - 1) / k) +
	    (k / 2 - bigger_num) * (n / k) - 1;

    /*
     * Note that select_order_statistic() has a side effect
     * of partitioning.
     */
    quantile[k / 2 - 1] = select_order_statistic(A, pivot_order, n);

    order[k / 2 - 1] = pivot_order;

    find_quantiles(A, pivot_order, quantile, order, k / 2);
    find_quantiles(
	    A + pivot_order + 1,
	    n - pivot_order - 1,
	    quantile + k / 2,
	    order + k / 2,
	    k  - k / 2);

    for (i = k / 2; i < k - 1; i++)
	order[i] += pivot_order + 1;
}

#define MAX_ELEM 50
#define MAX_SIZE 20

int main()
{
    elem_t *A, *B;
    elem_t *quantile;
    unsigned int *order;
    unsigned int i, k;
    unsigned int size;


    /* Preparation for invoking find_quantiles() */

    srand(time(NULL));

    size = rand() % MAX_SIZE + 1;
    A = (elem_t *) malloc(size * sizeof(elem_t));
    B = (elem_t *) malloc(size * sizeof(elem_t));

    printf("The original array A:\n");
    i = 0;
    do {
	A[i] = B[i] = rand() % (MAX_ELEM + 1);
	printf("[%u] %d\t", i, A[i]);
	++i;
	if (i % 7 == 0 && i < size)
	    putchar('\n');
    } while(i < size);
    printf("\n\n");

    k = 2 + ((unsigned int) rand()) % size;
    quantile = (elem_t *) malloc((k - 1) * sizeof(elem_t));
    order = (unsigned int *) malloc((k - 1) * sizeof(unsigned int));

    printf("The %uth quantiles of A is:\n", k);

    /* The tested function: find_quantiles() */

    find_quantiles(A, size, quantile, order, k);

    /* Print the result. */

    i = 0;
    do {
	printf("[%u] %d\t", order[i], quantile[i]);
	++i;
	if (i % 7 == 0 && i < k - 1)
	    putchar('\n');
    } while(i < k - 1);
    printf("\n\n");

    /* Sort the original array for checking. */
    qsort(B, (size_t) size, sizeof(int), compar);

    printf("After sorting, A becomes:\n");
    i = 0;
    do {
	printf("[%u] %d\t", i, B[i]);
	++i;
	if (i % 7 == 0 && i < size)
	    putchar('\n');
    } while(i < size);
    putchar('\n');

    free(order);
    free(quantile);
    free(B);
    free(A);

    return 0;
}

