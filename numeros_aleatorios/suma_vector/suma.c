#include <stdlib.h>
#include <stdio.h>

void relleno(int *puntero, int n);
void suma(int *puntero, int n, int h);

int main()
{
    int n = 10;
    int h = 1;
    int *vector = malloc(n * sizeof(int)); // Le asigna memoria al man, haber si estudia

    relleno(vector, n);
    suma(vector, n, h);

    free(vector); // Recreo

    return 0;
}

void relleno(int *puntero, int n)
{
    for(int i=0; i<n; i++)
    {
        *(puntero+i) = i;
    }

    return;
}

void suma(int *puntero, int n, int h)
{
    for(int i=0; i<n; i++)
    {
        *(puntero+i) = *(puntero+i) + h;
        printf("%d\n", *(puntero+i));
    }

    return;
}