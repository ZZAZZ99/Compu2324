#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define PI 3.14159265358979323846

int main(void) 
{
    double h, s, lambda, k, sigma, centro, *V, norma;
    double complex *psi, *chi, *beta, *alpha;
    int j, n, N, ciclos;

    FILE *NEKO = fopen("schrodinger_data.dat", "w");
    FILE *CONSTITUCION = fopen("norma.dat", "w");

    N = 1000;

    // Asignamos memoria dinámica

    psi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    chi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    beta = (double complex *)malloc(sizeof(double complex) * (N + 1));
    alpha = (double complex *)malloc(sizeof(double complex) * (N + 1));
    V = (double *)malloc(sizeof(double) * (N + 1));

    // Definimos constantes
    // Los ciclos los escoges tú (máximo N/4)

    n = 10000;
    h = 0.01;
    ciclos = 1.0;
    lambda = 1.0;
    k = 2.0 * ciclos * PI / (1.0 * N);
    s = 1.0 / (4.0 * k * k);
    sigma = N / 20.0;
    centro = N / 8.0;
    norma = 0.0;

    alpha[N-1] = 0.0;
    beta[N-1] = 0.0;
    chi[N] = 0.0;
    chi[0] = 0.0;

    // Condiciones de contorno de psi
    psi[0] = 0.0;
    psi[N] = 0.0;

    for (int j = 1; j < N; j++) 
    {
        psi[j] = cexp(1.0 * I * k * j) * cexp(-1.0 * (j - centro) * (j - centro) / (2.0 * sigma * sigma));
        norma += cabs(psi[j])*cabs(psi[j]);
    }

    for (int j = 1; j < N; j++) 
    {
        psi[j] = psi[j] / sqrt(norma);
    }

    // Calculamos V

    for (int j = 0; j < (2 * N / 5); j++) 
    {
        V[j] = 0.0;
    }

    for (int j = (2 * N / 5); j < (3 * N / 5) + 1; j++) {
        V[j] = lambda * k * k;
    }

    for (int j = (3 * N / 5) + 1; j < N + 1; j++) {
        V[j] = 0.0;
    }

    // Calculamos alfa
    for (int j = N - 2; j > 0; j--) {
        alpha[j-1] = -1.0 / ((-2.0 + (2.0 * I / s) - V[j]) + alpha[j]);
    }

    // Core del algoritmo
    for(int q=0; q<n+1; q++)
    {
        norma = 0.0;

        fprintf(NEKO, "\n");

        for (int j = N - 2; j > 0; j--) 
        {
            beta[j-1] = (1.0 / (-2.0 + 2.0 * (I/s) + alpha[j] - V[j]))*(4.0 * I * (psi[j]/s) - beta[j]);
        }

        for (int j = 0; j < N - 1; j++) 
        {
            chi[j+1] = alpha[j] * chi[j] + beta[j];
        }

        for (int j = 0; j < N+1; j++) 
        {
            psi[j] = chi[j] - psi[j];
        }

        // Imprimimos los datos en un archivo

        for (int i = 0; i < N + 1; i++) 
        {
            fprintf(NEKO, "%i, %f, %f, %f, %f\n", i, cabs(psi[i]), creal(psi[i]), cimag(psi[i]), V[i]);
            norma += cabs(psi[i])*cabs(psi[i]);
        }

        fprintf(NEKO, "\n");
        fprintf(CONSTITUCION, "%f\n", norma);

    }

    fclose(NEKO);

    free(psi);
    free(chi);
    free(beta);
    free(alpha);
    free(V);

    return 0;
}
