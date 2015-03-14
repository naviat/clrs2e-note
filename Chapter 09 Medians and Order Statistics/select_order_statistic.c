/*
 * select.c
 *
 * Implementing the SELECT algorithm described in Section 9.3
 * to select an order statistic.
 */

#include <stdlib.h>

typedef int elem_t;

static void
insertion_sort(elem_t *const A, const unsigned int size)
{
    unsigned int i, j;
    elem_t key;

    for (i = 1; i < size; i++) {
	key = A[i];
	for (j = i; j > 0 && A[j - 1] > key; j--)
	    A[j] = A[j - 1];
	A[j] = key;
    }
}

/*
 * partition
 *
 * Partition the array A into two parts around the pivot.
 * With the returned value i, A[0..i-1] are all less than
 * or equal to the pivot, while A[i..size-1] are all greater
 * than the pivot. Notice that pivot always exists in A,
 * which is guaranteed by SELECT algorithm.
 */
static
unsigned int
partition(
	elem_t *const A,
	const elem_t pivot,
       	const unsigned int size)
{
    unsigned int i, j;

    for (i = j = 0; j < size; j++) {
	if (A[j] <= pivot) {
	    elem_t tmp = A[i];
	    A[i++] = A[j];
	    A[j] = tmp;
	}
    }

    /*
     * Put the pivot x into A[i - 1]. It is guaranteed
     * by SELECT algorithm that there's at least one
     * element in A equalling the pivot. We must put it
     * in A[i - 1] to kick it out once the returned value
     * i is not the order wanted in the invocation of
     * select_order_statistic().
     */
    for (j = 0; j < i; j++) {
	if (A[j] == pivot) {
	    A[j] = A[i - 1];
	    A[i - 1] = pivot;
	    break;
	}
    }

    return i;
}

/*
 * select_order_statistic
 *
 * The SELECT algorithm. Find the ith order statistic in array
 * A[0..size-1]. Notice that 0 ≤ i < size.
 */
elem_t select_order_statistic(
	elem_t *const A,
       	const unsigned int i,
       	const unsigned int size)
{
    unsigned int u, v, w;
    elem_t *medians;
    elem_t median_median;
    unsigned median_num;

    if (size == 1)
	return A[0];

    /*
     * Divide the n elements of the input array into ⌊n/5⌋
     * groups of 5 elements each and at most one group
     * made up of the remaining n mod 5 elements. Then
     * find the median of each of the ⌈n/5⌉ groups by
     * first insertion sorting the elements of each group
     * (of which there are at most 5) and then picking the
     * median from the sorted list of group elements.
     */

    median_num = (size + 4) / 5;
    medians = (elem_t *) malloc(median_num * sizeof(elem_t));
    
    u = w = 0;
    do {
	v = u + 5 > size ? size : u + 5;
	insertion_sort(A + u, v - u);
	medians[w++] = A[ u + (v - u + 1) / 2 - 1 ];
	u = v;
    } while(v < size);

    /* the median-of-medians */
    median_median = select_order_statistic(medians,
	    (median_num + 1) / 2 - 1, median_num);

    free(medians);

    w = partition(A, median_median, size) - 1;

    if (i == w)
	return median_median;
    else if (i < w)
	/*
	 * The size must be reduced to w, but not (w + 1), that is,
	 * A[w] must not be included in the recursive call. Otherwise,
	 * the array size will never be reduced if the elements in
	 * A[0..w] are all the same, so that the recursive call will
	 * never end. Of course, we need to put median_median in A[w]
	 * to kick it out, which is done in partition().
	 */
	return select_order_statistic(A, i, w);
    else
	return select_order_statistic(A + w + 1, i - w - 1, size - w - 1);
}
