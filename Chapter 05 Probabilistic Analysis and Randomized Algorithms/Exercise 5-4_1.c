#include <math.h>
#include <stdio.h>

int main()
{
    double k, p2;
    const double n = 365.0;
    double f1, f2;

    printf("k = ");
    scanf("%lf", &k);

    /* Note that gcc will recognize the result of (n - 1) / n as
       an integer so it will always be 0. Thus 1 should be written
       as 1.0. */
    f1 = (n - 1.0) / n;
    f2 = (n - 1.0 + k) / n;
    p2 = pow(f1, k - 1.0) * f2;

    printf("p2 = %.2lf\n", p2);

    return 0;
}

