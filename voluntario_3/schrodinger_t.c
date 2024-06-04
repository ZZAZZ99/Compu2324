#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define PI 3.14159265358979323846

double nomeven_dere(double complex *psi, int N);
double real_aleatorio();

int main(void) 
{
    double h, s, lambda, k, sigma, centro, *V, norma, p_ante, p_carpe, aux;
    double complex *psi, *chi, *beta, *alpha;
    int j, n, N, ciclos, m_t, t;

    FILE *NEKO = fopen("schrodinger_data.dat", "w");
    FILE *CONSTITUCION = fopen("norma.dat", "w");

    N = 500;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

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
    ciclos = N/16;
    lambda = 0.3;
    k = 2.0 * ciclos * PI / (1.0 * N);
    s = 1.0 / (4.0 * k * k);
    sigma = N / 20.0;
    centro = N / 8.0;
    m_t = 0;
    aux = 0;

    alpha[N-1] = 0.0;

    // Calculamos V

    for (int j = 0; j < (2 * N / 5); j++) 
    {
        V[j] = 0.0;
    }

    for (int j = (2 * N / 5); j < (3 * N / 5) + 1; j++) 
    {
        V[j] = lambda * k * k;
    }

    for (int j = (3 * N / 5) + 1; j < N + 1; j++) 
    {
        V[j] = 0.0;
    }

    // Calculamos alfa
    for (int j = N - 2; j > 0; j--) 
    {
        alpha[j-1] = -1.0 / ((-2.0 + (2.0 * I / s) - V[j]) + alpha[j]);
    }

    // Condiciones de contorno para psi
    psi[0] = 0.0;
    psi[N] = 0.0;

    beta[N-1] = 0.0;
    chi[N] = 0.0;
    chi[0] = 0.0;
    norma = 0.0;

    for (int j = 1; j < N; j++) 
    {
        psi[j] = cexp(1.0 * I * k * j) * cexp(-1.0 * (j - centro) * (j - centro) / (2.0 * sigma * sigma));
        norma += cabs(psi[j])*cabs(psi[j]);
    }

    for (int j = 1; j < N; j++) 
    {
        psi[j] = psi[j] / sqrt(norma);
    }

    for(int w=0; w<100; w++)
    {
    
        t=0;
        p_ante = 0.0;
        p_carpe = 0.1;

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

            t++;

            // Calculamos la probabilidad a la derecha (primer máximo local t=742) (cuando el anterior deje de ser menor que el nuevo se llega al máximo local)
            p_carpe = nomeven_dere(psi, N);

            if(p_carpe < p_ante)
            {
                q = n+1;
            }
            else
            {
                p_ante = p_carpe;
            }
        
        }

        aux = real_aleatorio();

        if(aux > p_carpe)
        {
            m_t++;
        }

    }

    printf("El valor de K es: %f\n", m_t/100.0);

    fclose(NEKO);
    fclose(CONSTITUCION);

    free(psi);
    free(chi);
    free(beta);
    free(alpha);
    free(V);

    return 0;
}

//Función para el cálculo de la probabilidad a la derecha
double nomeven_dere(double complex *psi, int N)
{
    double probabilidad = 0.0;

    for (int i = 4*N/5; i < N + 1; i++) 
    {
        probabilidad += cabs(psi[i])*cabs(psi[i]);
    }

    return probabilidad;
}

double real_aleatorio()
{
    double numero;

    //Genero el número aleatorio con ayuda de rand
    numero = (double)(rand()) / RAND_MAX;

    return numero;
}
