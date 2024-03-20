#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define c (long double)(1.496 * pow(10, 11)) // Unidad astronómica en metros
#define G (long double)(6.674 * pow(10, -11)) // Constante de gravitación universal en m^3 kg^-1 s^-2
#define SOL (long double)(1.99 * pow(10, 30)) // Masa del sol en kg
#define MER (long double)(3.301 * pow(10, 23)) // Masa de mercurio en kg
#define VEN (long double)(4.867 * pow(10, 24)) // Masa de venus en kg
#define TIE (long double)(5.972 * pow(10, 24)) // Masa de la tierra en kg
#define MAR (long double)(6.417 * pow(10, 23)) // Masa de marte en kg

// Deberías ahora definir radios, distancias al Sol y tal para la coordenada x inicial de r.

void posicion(long double **x, long double **v, long double **a, double h);

int main()
{
    long double **r, **v, **a, *m, *t;
    double h;
    int filas, columnas;

    *t=0;    // Tiempo inicial siempre = 0
    h=0.01; // Salto, usar 1/100 o 1/1000
    filas = 4; // 4 planetas
    columnas = 2; // 2 dimensiones

    // Asignamos memoria a los arrays para las filas y columnas

    m = (long double *)malloc(sizeof(long double *));
    t = (long double *)malloc(sizeof(long double *));
    r = (long double **)malloc(filas *sizeof(long double *));
    v = (long double **)malloc(filas *sizeof(long double *));
    a = (long double **)malloc(filas *sizeof(long double *));

    // Reescalamiento de las masas

    m[0] = MER / SOL; 
    m[1] = VEN / SOL;
    m[2] = TIE / SOL;
    m[3] = MAR / SOL;

    for (int i = 0; i < filas; i++) {

        r[i] = (long double *)malloc(columnas *sizeof(long double *));
        v[i] = (long double *)malloc(columnas *sizeof(long double *));
        a[i] = (long double *)malloc(columnas *sizeof(long double *));
    }

    /* for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%Lf ", r[i][j]);
        }
        printf("\n");
    } */

    // posicion(r, v, a, h);

    // Liberamos la memoria de los arrays

    free(m);
    free(r);
    free(v);
    free(a);

    return 0;
}

// Funcion que calcula la posición siguiendo el algoritmo de Verlet (MAL)

void posicion(long double **r, long double **v, long double **a, double h)
{
    double rh;
    rh = **r + **v * h + 0.5 * **a * h * h;
    **r = rh;
}
