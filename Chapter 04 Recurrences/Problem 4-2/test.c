#include <stdio.h>
#include <stdlib.h>

extern unsigned int find_missing(char **A, unsigned int n);

int main()
{
    const unsigned int n = 7;
    char A0[7][3] = {
            { '0', '0', '0' },
            { '0', '1', '0' },
            { '1', '0', '1' },
            { '0', '1', '1' },
            { '0', '0', '1' },
            { '1', '1', '1' },
            { '1', '0', '0' } };
    
    char **A;
    int i, j;
    
    A = (char **) malloc(sizeof(char *) * n);
    for (i = 0; i < n; i++)
        A[i] = A0[i];
            
    printf("%u\n", find_missing(A, n));
    
    free(A);
 
    return 0;
}

