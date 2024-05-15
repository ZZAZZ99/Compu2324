#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    double complex a, b, c, d;
    a = I;
    b = cimag(I);
    c = a + b;
    d = a * b;
    printf("a = %f + %fi\n", creal(a), cimag(a));
    printf("b = %f + %fi\n", creal(b), cimag(b));
    printf("c = %f + %fi\n", creal(c), cimag(c));
    printf("d = %f + %fi\n", creal(d), cimag(d));





    return 0;
}