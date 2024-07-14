#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#define PI 3.14159265358979323846
#define gaussianas 1
#define width 100
#define sep 150
#define N (600+width+(gaussianas-1)*sep)

void nacimiento(double complex** psi, double ko);
void radiacion(double complex* gamma, double complex* alpha, double* V, double s);
void calculoB(double complex* b, double complex* psi, double s);
void calculobeta(double complex* beta, double complex* gamma, double complex* b);
double constitucion(double complex* vector);
double nomeven_dere(double complex* vector);

int main() 
{
    double *V, s, ko, nciclos, lambda, *norma2, *norma, p_dere, p_dere_aux, m_t, t, p, dist, almacen, h;
    double complex **psi, **alpha, **beta, **b, **Chi, **gamma;
    int encontrado;

    printf("N = %d, sep = %d, width = %d\n", N, sep, width);

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    //Asignamos memoria dinámica
    psi = (double complex**)malloc((N+1) * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        psi[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }

    alpha = (double complex**)malloc(N * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        alpha[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }
    
    beta = (double complex**)malloc(N * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        beta[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }

    b = (double complex**)malloc(N * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        b[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }

    Chi = (double complex**)malloc(N * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        Chi[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }

    gamma = (double complex**)malloc(N * sizeof(double complex*));
    for (int i = 0; i < gaussianas; i++) 
    {
        gamma[i] = (double complex*)malloc(gaussianas * sizeof(double complex));
    }

    V = (double*)malloc((N+1) * sizeof(double));
    norma2 = (double*)malloc(gaussianas * sizeof(double));
    norma = (double*)malloc(gaussianas * sizeof(double));

    // Definimos constantes
    // Los ciclos los escoges tú (máximo N/4)

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

    /*
    for (int i = 0; i < 300; i++) 
    {
        V[i] = 0;
    }
    for (int k = 0; k < barreras; k++) 
    {
        for (int i = 300 + k * width + k * dist; i < 300 + (k + 1) * width + (k + 1) * dist; i++) 
        {
            if (i > (300 + k * width + k * dist) && i < (300 + (k + 1) * width + k * dist)) 
            {
                V[i] = lambda * ko * ko;
            } 
            else 
            {
                V[i] = 0.0;
            }
        }
    }*/

    // Inicializa la función psi
    nacimiento(psi, ko);

    for(int z = 0; z < gaussianas; z++) 
    {

        norma2[z] = constitucion(psi[z]);

        for (int i = 0; i < N + 1; i++) 
        {
            psi[i][z] = psi[i][z];
        }

        // Calculo alpha y gamma
        radiacion(gamma[z], alpha[z], V, s);

        // Core del algoritmo
        for (int j = 0; j < N * 10; j++) 
        {
            calculoB(b[z], psi[z], s);
            calculobeta(beta[z], gamma[z], b[z]);

            Chi[0][z] = 0.0;

            for (int i = 0; i < N - 1; i++) 
            {
                Chi[i + 1][z] = alpha[i][z] * Chi[i][z] + beta[i][z];
            }

            for (int i = 0; i < N + 1; i++) 
            {
                psi[i][z] = Chi[i][z] - psi[i][z];
            }

            norma[z] = constitucion(psi[z]);

            p_dere_aux = nomeven_dere(psi[z]);
        
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

        p_dere = p_dere / norma[z];
    
        m_t = 0;

        for (int k = 0; k < 1000; k++) 
        {
            p = (double)rand() / RAND_MAX; //Genero un número aleatorio
        
            if (p < p_dere) 
            {
                m_t = m_t + 1;
            }
        }
    
        printf("%d: El valor de K es: %f\n", z, 1.0*m_t/1000.0); //1000 iteraciones
        printf("%d: El valor de la probabilidad a la derecha es: %f\n", z, p_dere);
    }

    // Liberamos memoria
    free(psi);
    free(alpha);
    free(beta);
    free(b);
    free(Chi);
    free(gamma);

    return 0;
}

void nacimiento(double complex** psi, double ko) 
{
    double sigma, centro, dist;

    dist = sep - width;

    sigma = N / 20.0;
    centro = N / 8.0;

    for (int k = 0; k < gaussianas; k++) 
    {
        psi[0][k] = 0.0;
        psi[N][k] = 0.0;

        for (int i = k * width + k * dist; i < (k + 1) * width + (k + 1) * dist; i++) 
        {
            if (i > (k * width + k * dist) && i < ((k + 1) * width + k * dist)) 
            {
                psi[i][k] = cexp(1.0 * I * ko * i) * cexp(-1.0 * (i - centro) * (i - centro) / (2.0 * sigma * sigma));
            } 
            else 
            {
                psi[i][k] = 0.0;
            }
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