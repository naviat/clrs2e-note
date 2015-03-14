/*
 * exercise_9_3_8.c
 *
 * The solution in instructor's manual can be generalized to finding the
 * $i$th order statistic of X[0..m-1] and Y[0..n-1], of which the running
 * time is O(\lg(mn)).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int elem_t;

/*
 * find_order_statistic
 *
 * Return the pointer to the element being the $i$th order statistic of
 * all the elements in X and Y, assuming the element stays in X[lowX..highX].
 * If the order statistic is not in X[lowX..highX], then NULL is returned.
 */
static elem_t*
find_order_statistic(
	const elem_t *const X,
	const unsigned int lowX,
	const unsigned int highX,
	const elem_t *const Y,
	const unsigned int sizeY,
	const unsigned int i)
{
    if (lowX > highX)
	return NULL;
    else {
	const unsigned int k = (lowX + highX) / 2;

	/* Check critical conditions first. */
	/* Both Y[ i - k - 1 ] and Y[ i - k ] do not exist. */
	if (k > i)    /* i - k < 0, k is too large. */
	    return (elem_t *) find_order_statistic(X, lowX, k - 1, Y, sizeY, i);
	else if (k + sizeY < i)    /* i - k - 1 > sizeY - 1, k is too small. */
	    return (elem_t *) find_order_statistic(X, k + 1, highX, Y, sizeY, i);
	/* Y[ i - k - 1 ] does not exist. */
	else if (k == i) {
	    if (X[k] <= Y[0])
		return (elem_t *) (X + k);
	    else
		return (elem_t *) find_order_statistic(X, lowX, k - 1, Y, sizeY, i);
	}
    	/* Y[ i - k ] does not exist. */
	else if (k + sizeY == i) {
	    if (X[k] >= Y[ sizeY - 1 ])
		return (elem_t *) (X + k);
	    else
		return (elem_t *) find_order_statistic(X, k + 1, highX, Y, sizeY, i);
	}
	/* Critical conditions checking completed. */
	else if (Y[ i - k - 1 ] <= X[k] && X[k] <= Y[ i - k ])
	    return (elem_t *) (X + k);
	else if (X[k] > Y[ i - k ])
	    return (elem_t *) find_order_statistic(X, lowX, k - 1, Y, sizeY, i);
	else
	    return (elem_t *) find_order_statistic(X, k + 1, highX, Y, sizeY, i);
    }
}

/*
 * two_array_order_statistic
 *
 * Return the $i$th order statistic of all the elements in X and Y.
 */
elem_t two_array_order_statistic(
	const elem_t *const X,
	const unsigned int sizeX,
	const elem_t *const Y,
	const unsigned int sizeY,
	const unsigned int i)
{
    elem_t *ret;
    ret = (elem_t *) find_order_statistic(X, 0, sizeX - 1, Y, sizeY, i);
    if (ret == NULL)
	ret = (elem_t *) find_order_statistic(Y, 0, sizeY - 1, X, sizeX, i);
    return *ret;
}

#define MAX_INCRE  10
#define MAX_SIZE   15

int main()
{
    elem_t *X, *Y;
    elem_t ret;
    unsigned int i, sizeX, sizeY;
    unsigned int j, l, r;

    /* Preparation for testing */

    srand(time(NULL));

    sizeX = ((unsigned int) rand()) % MAX_SIZE + 1;
    sizeY = ((unsigned int) rand()) % MAX_SIZE + 1;
    
    X = (elem_t *) malloc(sizeX * sizeof(elem_t));
    Y = (elem_t *) malloc(sizeY * sizeof(elem_t));

    X[0] = rand() % MAX_INCRE;
    for (j = 1; j < sizeX; j++)
	X[j] = X[j - 1] + rand() % MAX_SIZE;

    printf("X = [");
    for (j = 0; j < sizeX - 1; j++)
	printf("%d, ", X[j]);
    printf("%d]\n", X[sizeX - 1]);

    Y[0] = rand() % MAX_INCRE;
    for (j = 1; j < sizeY; j++)
	Y[j] = Y[j - 1] + rand() % MAX_SIZE;

    printf("Y = [");
    for (j = 0; j < sizeY - 1; j++)
	printf("%d, ", Y[j]);
    printf("%d]\n\n", Y[sizeY - 1]);

    i = (unsigned int) rand() % (sizeX + sizeY);
    printf("The %uth order statistic of all elements in X and Y is: ", i);
    fflush(stdout);

    ret = two_array_order_statistic(X, sizeX, Y, sizeY, i);    /* the tested function */

    printf("%d\n\n", ret);

    /* Print the merged sorted list of X and Y for checking. */

    printf("The merged sorted list of X and Y is:\n");

    j = l = r = 0;
    while (l < sizeX && r < sizeY) {
	if (X[l] <= Y[r])
	    printf("[%u] %d  ", j++, X[l++]);
	else
	    printf("[%u] %d  ", j++, Y[r++]);
    }
    
    if (l < sizeX) {
	do {
	    printf("[%u] %d  ", j++, X[l++]);
	} while(l < sizeX);
    }
    else {
	do {
	    printf("[%u] %d  ", j++, Y[r++]);
	} while(r < sizeY);
    }
    putchar('\n');

    free(Y);
    free(X);

    return 0;
}

