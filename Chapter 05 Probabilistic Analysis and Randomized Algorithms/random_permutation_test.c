#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 8

int main()
{
    int A[N];
    unsigned int i;

    printf("The initial array A : ");
    for (i = 0; i < N; i++) {
        A[i] = i;
        printf("%d ", A[i]);
    }
    putchar('\n');

    srand(time(NULL));
    randomize_in_place(A, N);

    printf("after randomize_in_place() : ");
    for (i = 0; i < N; i++)
        printf("%d ", A[i]);
    putchar('\n');

    return 0;
}

