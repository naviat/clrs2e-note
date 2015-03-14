#include <stdio.h>
#include <stdlib.h>

#define M 5
#define N 4

extern unsigned int *left_min_indices(double **A, unsigned int m, unsigned int n);

int main()
{
    double A0[M][N] = {
            { 37, 23, 22, 32 },
            { 21,  6,  5, 10 },
            { 53, 34, 30, 31 },
            { 32, 13,  9,  6 },
            { 43, 21, 15,  8 } };

    double **A;
    unsigned int *lm_min_indices;

    unsigned int i, j;

    A = (double **) malloc(M * sizeof(double *));
    for (i = 0; i < M; i++) {
        A[i] = (double *) malloc(N * sizeof(double));
        for (j = 0; j < N; j++)
            A[i][j] = A0[i][j];
    }

    if (is_Monge(A, M, N)) {
        printf("This is a Monge array.\n");
        lm_min_indices = left_min_indices(A, M, N);
        printf("The indices of the column of the leftmost minimums are:\n");
        for (i = 0; i < M; i++)
            printf("%4u", lm_min_indices[i]);
        putchar('\n');
        free(lm_min_indices);
    }
    else
        printf("This is not a Monge array.\n");

    for (i = 0; i < M; i++)
        free(A[i]);
    free(A);

    return 0;
}

