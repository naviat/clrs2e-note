#include <stdlib.h>

/* lmin_indices_recur
 *
 * This function is the real procedure that computes the indices of the column of
 * the leftmost minimum elements using the divide-and-conquer algorithm described
 * in (d).
 *
 */
static unsigned int *lmin_indices_recur(double **A, unsigned int m, unsigned int n)
{
    unsigned int *indices;

    double **A_oddr;    /* pointers to the odd-indexed rows of A */
    unsigned int *indices_oddr;

    double min;
    unsigned int i, j, s, t, lmin_index;

    indices = (unsigned int *) malloc(m * sizeof(unsigned int));

    if (m == 1) {
        min = A[0][0];
        for (j = 1; j < n; j++)
            if (A[0][j] < min) {
                lmin_index = j;
                min = A[0][j];
            }
        indices[0] = lmin_index;
    }
    else {

        /* Compute the leftmost minimum in the odd-indexed rows of A. */

        A_oddr = (double **) malloc(m / 2 * sizeof(double *));
        for (i = 1; i < m; i += 2)
            A_oddr[i / 2] = A[i];
        indices_oddr = lmin_indices_recur(A_oddr, m / 2, n);
        for (i = 1; i < m; i += 2)
            indices[i] = indices_oddr[i / 2];
        free(indices_oddr);
        free(A_oddr);

        /* Compute the leftmost minimum in the even-indexed rows of A. */

        s = 0;
        for (i = 0; i < m; i += 2) {
            t = i < m - 1 ? indices[i + 1] : n - 1;
            min = A[i][s];
            lmin_index = s;
            for (j = s + 1; j <= t; j++)
                if (A[i][j] < min) {
                    lmin_index = j;
                    min = A[i][j];
                }
            indices[i] = lmin_index;
            s = t;
        }
    }

    return indices;
}

/* is_Monge
 *
 * Determine that whether an array of real numbers is Monge.
 *
 * A    the array to be judged
 * m    number of rows
 * n    number of columns
 */
int is_Monge(double **A, unsigned int m, unsigned int n)
{
    unsigned int i, j;

    if (m <= 1 || n <= 1)
        return 0;

    for (i = 0; i < m - 1; i++)
        for (j = 0; j < n - 1; j++)
            if (A[i][j] + A[i + 1][j + 1] > A[i][j + 1] + A[i + 1][j])
                return 0;

    return 1;
}

/* left_min_indices
 *
 * Computes the indices of the column of the leftmost minimum elements of a Monge array.
 *
 * A    the Monge array to be computed on
 * m    the number of rows in A
 * n    the number of columns in A
 *
 * return    an array containing the requested indices. Note that the array needs to be
 *           freed manually after being used.
 *           NULL if A is not Monge.
 *
 */
unsigned int *left_min_indices(double **A, unsigned int m, unsigned int n)
{
    if (!is_Monge(A, m, n))
        return NULL;
    return lmin_indices_recur(A, m, n);
}

