#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define PI 3.14159265358979323846

double nomeven_dere(double complex *psi, int N, int barreras, int A);
double nomeven_izq(double complex *psi, int N);
double real_aleatorio();

int main(void) 
{
    double h, s, lambda, k, sigma, centro, *V, norma, norma2, p, p_dere, p_dere_media, p_izq, almacen;
    double complex *psi, *chi, *beta, *alpha;
    int j, N, ciclos, m_t, t, ND, encontrado, A, barreras, m;

    N = 1000;
    barreras = 1;
    A = N + 2*N*(barreras-1)/5;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    //Ficheros

    // Asignamos memoria dinámica

    psi = (double complex *)malloc(sizeof(double complex) * (A + 1));
    chi = (double complex *)malloc(sizeof(double complex) * (A + 1));
    beta = (double complex *)malloc(sizeof(double complex) * (A + 1));
    alpha = (double complex *)malloc(sizeof(double complex) * (A + 1));
    V = (double *)malloc(sizeof(double) * (A + 1));

    // Definimos constantes
    // Los ciclos los escoges tú (máximo N/4)

    ciclos = A/4;
    lambda = 0.5;
    k = 2.0 * ciclos * PI / (1.0 * A);
    s = 1.0 / (4.0 * k * k);
    sigma = A / 20.0;
    centro = A / 8.0;
    m_t = 0;
    p_dere_media = 0.0;
    ND = round(2000*(barreras/2.0));
    h = 2*k;

    alpha[A-1] = 0.0;

    // Calculamos V
    //Antes de las barreras
    for (int j = 0; j < (N/5) + 1; j++) 
    {
        V[j] = 0.0;
    }

    //Las barreras
    for(int j = 0; j < 2*barreras + 1; j++)
    {
        for (int i = 0; i < (N/5) + 1; i++) 
        {
            if(j%2 == 0)
            {
                m = i + (j+1)*N*0.2;
                V[m] = 0.0;
            }
            else
            {
                m = i + (j+1)*N*0.2;
                V[m] = lambda*k*k;
            }

        }
    }
    //Después de las barreras
    for (int j = (2*barreras+1)*N/5; j< A + 1; j++) 
    {
        V[j] = 0.0;
    }

    // Calculamos alfa
    for (int j = A - 2; j > 0; j--) 
    {
        alpha[j-1] = -1.0 / ((-2.0 + (2.0 * I / s) - V[j]) + alpha[j]);
    }

    // Realizamos 1000 iteraciones
    for(int w=1; w<1001; w++)
    {

        // Condiciones de contorno para psi
        psi[0] = 0.0;
        psi[A] = 0.0;

        encontrado = 0;
        t = 1;

        beta[A-1] = 0.0;
        chi[A] = 0.0;
        chi[0] = 0.0;
        norma = 0.0;

        for (int j = 1; j < A; j++) 
        {
            psi[j] = cexp(1.0 * I * k * j) * cexp(-1.0 * (j - centro) * (j - centro) / (2.0 * sigma * sigma));
            norma += cabs(psi[j])*cabs(psi[j]);
        }

        for (int j = 1; j < A; j++) 
        {
            psi[j] = psi[j] / sqrt(norma);
        }

        // Core del algoritmo
        while(encontrado == 0)
        {
            t++;

            norma = 0.0;

            chi[A] = 0.0;
            chi[0] = 0.0;

            for (int j = A - 2; j > 0; j--) 
            {
                beta[j-1] = (1.0 / (-2.0 + 2.0 * (I/s) + alpha[j] - V[j]))*(4.0 * I * (psi[j]/s) - beta[j]);
            }

            for (int j = 0; j < A - 1; j++) 
            {
                chi[j+1] = alpha[j] * chi[j] + beta[j];
            }

            for (int j = 0; j < A+1; j++) 
            {
                psi[j] = chi[j] - psi[j];
            }

            if((t%ND) == 0)
            {
                p_dere = 0.0;
                p_dere += nomeven_dere(psi, N, barreras, A);
                p_dere_media += p_dere;

                p = real_aleatorio(); //Genero un número aleatorio

                if(p < p_dere)
                {
                    m_t++;
                    encontrado = 1;
                }
                else
                {
                    norma2 = 0.0;

                    for (int j = 2*(barreras+1)*N/5; j < A; j++) 
                    {
                        psi[j] = 0.0;
                    }

                    for (int j = 1; j < A; j++) 
                    {
                        norma2 += cabs(psi[j])*cabs(psi[j]);
                    } 

                    for (int j = 1; j < A; j++) 
                    {
                        psi[j] = psi[j] / sqrt(norma);
                    }

                    //Compruebo si se encuentra a la izquierda
                    
                    p_izq = 0.0;
                    p_izq += nomeven_izq(psi, N);

                    p = real_aleatorio(); //Genero un número aleatorio

                    if(p < p_izq)
                    {
                        encontrado = 1; //Se sale del bucle (está a la izquierda)
                    }
                    else //No se detecta en ninguna parte
                    {
                        norma2 = 0.0;

                        for (int j = 1; j <= N/5; j++) 
                        {
                            psi[j] = 0.0;
                        }

                        for (int j = 1; j < A; j++) 
                        {
                            norma2 += cabs(psi[j])*cabs(psi[j]);
                        }

                        for (int j = 1; j < A; j++) 
                        {
                            psi[j] = psi[j] / sqrt(norma);
                        } 
                    }
                }
            }

            if(t>4*ND)
            {
                encontrado = 1; //Si no se detecta en 4*nD iteraciones, se sale del bucle
            }
        }

    }

    printf("El valor de K es: %f\n", 1.0*m_t/1000.0); //1000 iteraciones
    printf("El valor de la probabilidad a la derecha es: %f\n", p_dere_media/1000.0);

    free(psi);
    free(chi);
    free(beta);
    free(alpha);
    free(V);

    return 0;
}

//Función para el cálculo de la probabilidad a la derecha
double nomeven_dere(double complex *psi, int N, int barreras, int A)
{
    double probabilidad = 0.0;

    for (int i = (N/5)*2*(barreras+1); i < A; i++) 
    {
        probabilidad += cabs(psi[i])*cabs(psi[i]);
    }

    return probabilidad;
}

//Función para el cálculo de la probabilidad a la izquierda
double nomeven_izq(double complex *psi, int N)
{
    double probabilidad = 0.0;

    for (int i = 1; i <= N/5; i++) 
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
