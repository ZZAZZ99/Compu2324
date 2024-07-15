#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#define PI 3.14159265358979323846
#define barreras 1
#define width 100
#define sep 150
#define N (600+width+(barreras-1)*sep)
#define M 1000000  // Define el número de Gaussianas

void nacimiento(double complex* psi, double ko);
void radiacion(double complex* gamma, double complex* alpha, double* V, double s);
void calculoB(double complex* b, double complex* psi, double s);
void calculobeta(double complex* beta, double complex* gamma, double complex* b);
double constitucion(double complex* vector);
double nomeven_dere(double complex* vector);

int main() 
{
    double *V, s, ko, nciclos, lambda, norma2, norma, p_dere, p_dere_aux, m_t, t, p, dist, almacen, h;
    double complex *psi, *alpha, *beta, *b, *Chi, *gamma;
    int encontrado;

    //printf("N = %d, sep = %d, width = %d\n", N, sep, width);

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    //Asignamos memoria dinámica
    psi = (double complex*)malloc((N+1) * sizeof(double complex));
    alpha = (double complex*)malloc(N * sizeof(double complex));
    beta = (double complex*)malloc(N * sizeof(double complex));
    b = (double complex*)malloc(N * sizeof(double complex));
    Chi = (double complex*)malloc(N * sizeof(double complex));
    gamma = (double complex*)malloc(N * sizeof(double complex));
    V = (double*)malloc((N+1) * sizeof(double));

    // Definimos constantes
    encontrado = 0;
    p_dere = 0;
    lambda = 0.3;
    nciclos = N / 5;
    ko = 2 * PI * nciclos / N;
    s = 1 / (4.0 * ko * ko);
    dist = sep - width;
    h = 2*ko;

    // Inicialización del potencial
    for (int j = 0; j < (2 * N / 5); j++) 
    {
        V[j] = 0.0;
    }

    for (int j = (2 * N / 5); j < (3 * N / 5) + 1; j++) 
    {
        V[j] = lambda * ko * ko;
    }

    for (int j = (3 * N / 5) + 1; j < N + 1; j++) 
    {
        V[j] = 0.0;
    }

    // Inicializa la función psi
    nacimiento(psi, ko);
    
    norma2 = constitucion(psi);

    for (int i = 0; i < N + 1; i++) 
    {
        psi[i] = psi[i];
    }

    // Calculo alpha y gamma
    radiacion(gamma, alpha, V, s);

    // Core del algoritmo
    for (int j = 0; j < N * 10; j++) 
    {
        calculoB(b, psi, s);
        calculobeta(beta, gamma, b);

        Chi[0] = 0.0;

        for (int i = 0; i < N - 1; i++) 
        {
            Chi[i + 1] = alpha[i] * Chi[i] + beta[i];
        }

        for (int i = 0; i < N + 1; i++) 
        {
            psi[i] = Chi[i] - psi[i];
        }

        norma = constitucion(psi);

        p_dere_aux = nomeven_dere(psi);
        
        if ((p_dere_aux >= p_dere) && (encontrado == 0)) 
        {
            p_dere = p_dere_aux;
            t = j;
        } 
        else if ((p_dere - p_dere_aux) > 0.001) 
        {
            encontrado = 1;
        }
    }

    p_dere = p_dere / norma;
    
    m_t = 0;

    for (int k = 0; k < 1000; k++) 
    {
        p = (double)rand() / RAND_MAX; //Genero un número aleatorio
        
        if (p < p_dere) 
        {
            m_t = m_t + 1;
        }
    }
    
    printf("El valor de K es: %f\n", 1.0*m_t/1000.0); //1000 iteraciones
    printf("El valor de la probabilidad a la derecha es: %f\n", p_dere);

    free(psi);
    free(alpha);
    free(beta);
    free(b);
    free(Chi);
    free(gamma);
    free(V);

    return 0;
}

void nacimiento(double complex* psi, double ko) 
{
    double sigma, centro;
    double dist = N / (M + 1); // Distancia entre los centros de las Gaussianas

    psi[0] = 0.0;
    psi[N] = 0.0;

    sigma = N / 20.0;

    // Inicializamos psi con M Gaussianas
    for (int m = 0; m < M; m++) 
    {
        centro = dist * (m + 1); // Centro de la m-ésima Gaussiana

        for (int i = 1; i < N; i++) 
        {
            psi[i] += cexp(1.0 * I * ko * i) * cexp(-1.0 * (i - centro) * (i - centro) / (2.0 * sigma * sigma));
        }
    }
}

void radiacion(double complex* gamma, double complex* alpha, double* V, double s) 
{
    double complex a0[N];

    alpha[N - 1] = 0.0;

    for (int i = N - 1; i > 0; i--) 
    {
        a0[i] = -2.0 - V[i] + 2.0 * I / s;
        gamma[i] = 1.0 / (a0[i] + alpha[i]);
        alpha[i - 1] = -gamma[i];
    }
}

void calculoB(double complex* b, double complex* psi, double s) 
{
    double reales;
    double imag;

    for (int i = 0; i < N; i++) 
    {
        reales = -4 * cimag(psi[i]) / s;
        imag = 4 * creal(psi[i]) / s;
        b[i] = reales + imag * I;
    }
}

void calculobeta(double complex* beta, double complex* gamma, double complex* b) 
{
    beta[N - 1] = 0.0 + 0.0 * I;

    for (int i = N - 1; i > 0; i--) 
    {
        beta[i - 1] = gamma[i] * (b[i] - beta[i]);
    }
}

double constitucion(double complex* vector) 
{
    double norma = 0.0;

    for (int i = 0; i < N + 1; i++) 
    {
        norma += cabs(vector[i]) * cabs(vector[i]);
    }

    return norma;
}

double nomeven_dere(double complex* vector) 
{
    double suma = 0;

    for (int i = N - 100; i < N + 1; i++) 
    {
        suma += cabs(vector[i]) * cabs(vector[i]);
    }

    return suma;
}
