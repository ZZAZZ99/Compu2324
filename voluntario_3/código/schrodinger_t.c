#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#define PI 3.14159265358979323846
#define barreras 1
#define width 100
#define sep 150
#define N 3500//(600+width+(barreras-1)*sep)

void nacimiento(double complex* psi, double ko);
void radiacion(double complex* gamma, double complex* alpha, double* V, double s);
void calculoB(double complex* b, double complex* psi, double s);
void calculobeta(double complex* beta, double complex* gamma, double complex* b);
double constitucion(double complex* vector);
double nomeven_dere(double complex* vector);
void barrelroll(double complex *psi, double complex *psi2);
double posicion(double complex *vector);
double cinetica(double complex *D2psi, double complex *psi2);
double desviacion(double *vector, double media);
void derivacion(double complex *vector, double complex *derivada);

int main() 
{
    double *V, s, ko, nciclos, lambda, norma2, norma, p_dere, p_dere_aux, m_t, t, p, dist, almacen, h, *x, *T, e_x, e_T, mediax, mediaT;
    double complex *psi, *psi2, *alpha, *beta, *b, *Chi, *gamma, *D2psi;
    int encontrado;

    printf("N = %d, sep = %d, width = %d\n", N, sep, width);

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    //Ficheros
    FILE *PELI = fopen("posicion.dat", "w");
    FILE *CINE = fopen("cinetica.dat", "w");

    fprintf(PELI, "x;y\n"); //Cabecera de los ficheros para generar un plot
    fprintf(CINE, "x;y\n");

    //Asignamos memoria dinámica
    psi = (double complex*)malloc((N+1) * sizeof(double complex));
    psi2 = (double complex*)malloc((N+1) * sizeof(double complex));
    alpha = (double complex*)malloc(N * sizeof(double complex));
    beta = (double complex*)malloc(N * sizeof(double complex));
    b = (double complex*)malloc(N * sizeof(double complex));
    Chi = (double complex*)malloc(N * sizeof(double complex));
    gamma = (double complex*)malloc(N * sizeof(double complex));
    V = (double*)malloc((N+1) * sizeof(double));
    D2psi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    x = (double*)malloc(10*N * sizeof(double));
    T = (double*)malloc(10*N * sizeof(double));

    // Definimos constantes
    // Los ciclos los escoges tú (máximo N/4)

    encontrado = 0;
    p_dere = 0;
    lambda = 0.3;
    nciclos = 50; //N / 5; //N/100;
    ko = 2 * PI * nciclos / N;
    s = 1 / (4.0 * ko * ko);
    dist = sep - width;
    h = 2*ko;

    mediax = 0.0;
    mediaT = 0.0;

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

        //Cálculo de valor esperado de la posición y energía cinética

        barrelroll(psi, psi2);
        derivacion(psi, D2psi);
        derivacion(D2psi, D2psi);

        x[j] = posicion(psi);
        mediax += x[j];

        T[j] = cinetica(D2psi, psi2);
        mediaT += T[j];

        fprintf(PELI, "%d;%f\n", j, x[j]);
        fprintf(CINE, "%d;%f\n", j, T[j]);
    }

    mediax = mediax / (N * 10);
    mediaT = mediaT / (N * 10);

    e_x = desviacion(x, mediax);
    e_T = desviacion(T, mediaT);

    printf("El valor esperado de la posición es: %f;%f\n", mediax, e_x);
    printf("El valor esperado de la energía cinética es: %f;%f\n", mediaT, e_T);
    
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
    free(psi2);
    free(alpha);
    free(beta);
    free(b);
    free(Chi);
    free(gamma);
    free(V);
    free(D2psi);
    free(x);
    free(T);

    fclose(PELI);
    fclose(CINE);

    return 0;
}

void nacimiento(double complex* psi, double ko) 
{
    double sigma, centro;

    psi[0] = 0.0;
    psi[N] = 0.0;

    sigma = N / 20.0;
    centro = N / 8.0;

    for (int i = 1; i < N; i++) 
    {
        psi[i] = cexp(1.0 * I * ko * i) * cexp(-1.0 * (i - centro) * (i - centro) / (2.0 * sigma * sigma));
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

void barrelroll(double complex *psi, double complex *psi2)
{
    double preal, pimag;

    for (int i = 0; i < N+1; i++)
    {
        preal = creal(psi[i]);
        pimag = -cimag(psi[i]);
        psi2[i] = preal + I*pimag;
    }
    
    return ;
}

double posicion(double complex *vector)
{
    double suma, norma;

    suma=0;
    norma=0;

    for (int i = 0; i < N+1; i++)
    {
        suma += i*cabs(vector[i]);
        norma += cabs(vector[i]);
    }
    
    return suma/norma;
}

double cinetica(double complex *D2psi, double complex *psi2)
{
    double suma=0;
    double norma=0;
    for (int i = 0; i < N+1; i++)
    {
        suma += -creal(psi2[i]*D2psi[i]);
        norma += cabs(psi2[i]);
    }

    return suma/(2*norma); 
}

double desviacion(double *vector, double media)
{
    double suma;
    suma = 0.0;

    for (int i = 0; i < 10*N; i++)
    {
        suma += pow((vector[i] - media),2);
    }

    return sqrt(suma/N);
}

void derivacion(double complex *vector, double complex *derivada)
{
    for (int i = 0; i < N+1; i++)
    {
        if (i==N)
        {
            derivada[i] = 0.0;
        }
        else
        {
            derivada[i] = vector[i+1] - vector[i];
        }
        
    }

    return;
}
