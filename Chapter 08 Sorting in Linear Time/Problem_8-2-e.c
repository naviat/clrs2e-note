/*
 * Counting sort in place. Note that this sorting alrogithm is not stable.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 10
#define MAX_K 20

typedef struct {
    unsigned int key;
    size_t index;
}elem_t;

int main()
{
    unsigned int k;
    size_t n;
    elem_t *A;    /* the input array */
    size_t *C, *D, *E;
    
    size_t i;
    unsigned int j;
    
    /* Initialization */
    
    srand(time(NULL));
    k = ((unsigned int) rand() + (unsigned int) rand()) % MAX_K + 1;
    n = ((size_t) rand() + (size_t) rand()) % MAX_N + 1;
    printf("n = %u, k = %u\n", n, k);
    
    A = (elem_t *) malloc(sizeof(elem_t) * n);

    C = (unsigned int *) malloc(sizeof(unsigned int) * (k + 1));    /* C[0] is unused */
    D = (unsigned int *) malloc(sizeof(unsigned int) * (k + 1));    /* D[0] is unused */
    E = (unsigned int *) malloc(sizeof(unsigned int) * (k + 1));    /* E[0] is unused */

    printf("Before sorting, the keys and indices of A are\nkeys:\t\t{");
    for (i = 0; i < n - 1; i++) {
        A[i].key = ((unsigned int) rand() + (unsigned int) rand()) % k + 1;
        printf("%u,\t", A[i].key);
    }
    A[n - 1].key = ((unsigned int) rand() + (unsigned int) rand()) % k + 1;
    printf("%u}\nindices:\t{", A[n - 1].key);
    for (i = 0; i < n - 1; i++) {
        A[i].index = i;
        printf("%u,\t", A[i].index);
    }
    A[n - 1].index = n - 1;
    printf("%u}\n", A[n - 1].index);
    
    /* The real sorting algorithm */
    
    for (i = 1; i <= k; i++)
        C[i] = D[i] = E[i] = 0;
    for (i = 0; i < n; i++)
        ++C[A[i].key], ++D[A[i].key], ++E[A[i].key];
    for (i = 2; i <= k; i++) {
        C[i] += C[i - 1];
        E[i] += E[i - 1];
    }

    i = n;
    j = k;
    while (i > 0) {
        --i;

        /* 
         * This inner loop iterates k times in the whole
         * excecution of the algorithm in the worst case.
         */
        while (D[j] == 0)
            --j;

        /* 
         * Swap A[i] into its correct position until A[i] 
         * holds the correct element. This inner loop iterates 
         * n times in the whole execution of the algorithm in 
         * the worst case.
         */
        while (A[i].key != j) {
            const elem_t tmp = A[i];
            A[i] = A[C[tmp.key] - 1];
            A[C[tmp.key] - 1] = tmp;
            --C[tmp.key];
        }
        if (i == E[j - 1])
            --j;
    };
    
    /* Sorting completed */
    
    printf("After sorting, the keys and indices of A are\nkeys:\t\t{");
    for (i = 0; i < n - 1; i++)
        printf("%u,\t", A[i].key);
    printf("%u}\nindices:\t{", A[n - 1].key);
    for (i = 0; i < n - 1; i++)
        printf("%u,\t", A[i].index);
    printf("%u}\n", A[n - 1].index);
    
    free(E);
    free(D);
    free(C);
    free(A);
    
    return 0;
}

