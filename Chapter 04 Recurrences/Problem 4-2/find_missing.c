#define _ISOC99_SOURCE    // in order to use the log2() function 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * finding_missing
 *
 * Find the missing integer in an array containing all the integers from 0 to
 * n except one.
 *
 * A	the array containing all the integers from 0 to n except one
 * n	the max integer that is possible stored in array A
 *
 * return	the missing integer
 */
unsigned int find_missing(char **A, unsigned int n)
{
    int i, m;
    char **A0;    // storing the pointers which point to the integers whose lowest bits are 0
    char **A1;    // storing the pointers which point to the integers whose lowest bits are 1
    char **A0ptr, **A1ptr;
    
    unsigned int missing_bit;
    unsigned int ret;
    
    if (n <= 1) {
        return A[0][0] == '0' ? 1 : 0;
    }
    
    m = (int) (ceil(log2((double) (n + 1))) - 1);
    A0 = A0ptr = (char **) malloc(sizeof(char *) * (n / 2 + 1));
    A1 = A1ptr = (char **) malloc(
            sizeof(char *) * (unsigned int) ceil((double)n / 2));
    
    for (i = 0; i < n; i++)
        if (A[i][m] == '0')
            *(A0ptr++) = A[i];
        else
            *(A1ptr++) = A[i];
    
    if (A0ptr - A0 > A1ptr - A1) {
    // the missing integer's lowest bit must be 1
        missing_bit = 1;
        free(A0);
        ret = find_missing(A1, A1ptr - A1);
        free(A1);
    }
    else {
    // the missing integer's lowest bit must be 0
        missing_bit = 0;
        free(A1);
        ret = find_missing(A0, A0ptr - A0);
        free(A0);
    }
    
    return (ret << 1) | missing_bit;
}

