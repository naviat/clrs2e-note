#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9

void random_permute(int *A, unsigned int n)
{
    unsigned int i, j;
    const unsigned int m = n - 1;
    
    int tmp;
    
    for (i = 0; i < m; i++) {
        j = i + rand() % (n - i);
        
        /* exchange A[i] and A[j] */
        tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
    }
}

int main()
{
    int A[N] = {5, 13, 2, 25, 7, 17, 20, 8, 4};

    unsigned int i;

    if (is_max_heap(A, N))
        printf("A is a max-heap now.\n");
    else
        printf("A is still not a max-heap.\n");

    if (is_max_heap(A, N))
        printf("A is a max-heap.\n");
    else
        printf("A is not a max-heap.\n");

    max_heapify(A, 1, N);

    printf("After calling max_heapify() on A,\nA = {");
    for (i = 0; i < N - 1; i++)
        printf("%d, ", A[i]);
    printf("%d}.\n", A[N - 1]);

    if (is_max_heap(A, N))
        printf("A is a max-heap now.\n");
    else
        printf("A is still not a max-heap.\n");
        
    srand(time(NULL));

    printf("Randomly permute A.\n");
    do {
        random_permute(A, N);
    } while(is_max_heap(A, N));
    printf("Now, A = {");
    for (i = 0; i < N - 1; i++)
        printf("%d, ", A[i]);
    printf("%d}, and A is not a max-heap.\n", A[N - 1]);
    
    build_max_heap(A, N);
    
    printf("After calling build_max_heap() on A,\nA = {");
    for (i = 0; i < N - 1; i++)
        printf("%d, ", A[i]);
    printf("%d}.\n", A[N - 1]);

    if (is_max_heap(A, N))
        printf("A is surely a max-heap now.\n");
    else
        printf("What?! Impossible! A is still not a max-heap! Check the source code!\n");

    random_permute(A, N);

    printf("Randomly permute A again.\n");
    printf("Now, A = {");
    for (i = 0; i < N - 1; i++)
        printf("%d, ", A[i]);
    printf("%d}\n", A[N - 1]);

    heapsort(A, N);

    printf("After calling heapsort() on A, A = {");
    for (i = 0; i < N - 1; i++)
        printf("%d, ", A[i]);
    printf("%d}\n", A[N - 1]);

    return 0;
}

