#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define PI 3.14159265358979323846

double nomeven_dere(double complex *psi, int N);
double nomeven_izq(double complex *psi, int N);
double real_aleatorio();

int main(void) 
{
    double h, s, lambda, k, sigma, centro, *V, norma, norma2, p, p_dere, p_dere_media, p_izq, almacen;
    double complex *psi, *chi, *beta, *alpha, *D2psi, x, T, e_x;
    int j, N, ciclos, m_t, t, ND, encontrado;

    N = 1000;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    //Ficheros
    FILE *PELI = fopen("posicion.dat", "w");
    FILE *CINE = fopen("cinetica.dat", "w");

    fprintf(PELI, "x;y;dy\n"); //Cabecera del fichero para generar un plot

    // Asignamos memoria dinámica

    psi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    D2psi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    chi = (double complex *)malloc(sizeof(double complex) * (N + 1));
    beta = (double complex *)malloc(sizeof(double complex) * (N + 1));
    alpha = (double complex *)malloc(sizeof(double complex) * (N + 1));
    V = (double *)malloc(sizeof(double) * (N + 1));

    // Definimos constantes
    // Los ciclos los escoges tú (máximo N/4)

    ciclos = 50;
    lambda = 0.3;
    k = 2.0 * ciclos * PI / (1.0 * N);
    s = 1.0 / (4.0 * k * k);
    sigma = N / 20.0;
    centro = N / 8.0;
    m_t = 0;
    p_dere_media = 0.0;
    ND = 500;
    h = 2*k;

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

    // Realizamos 1000 iteraciones
    for(int w=1; w<2; w++)
    {

        // Condiciones de contorno para psi
        psi[0] = 0.0;
        psi[N] = 0.0;

        encontrado = 0;
        t = 1;

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

        // Core del algoritmo
        while(encontrado == 0)
        {
            t++;

            norma = 0.0;

            chi[N] = 0.0;
            chi[0] = 0.0;

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

            if((t%ND) == 0)
            {
                p_dere = 0.0;
                p_dere += nomeven_dere(psi, N);
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

                    for (int j = 4*N/5; j < N; j++) 
                    {
                        psi[j] = 0.0;
                    }

                    for (int j = 1; j < N; j++) 
                    {
                        norma2 += cabs(psi[j])*cabs(psi[j]);
                    } 

                    for (int j = 1; j < N; j++) 
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

                        for (int j = 1; j < N; j++) 
                        {
                            norma2 += cabs(psi[j])*cabs(psi[j]);
                        }

                        for (int j = 1; j < N; j++) 
                        {
                            psi[j] = psi[j] / sqrt(norma);
                        } 
                    }
                }
            }

            //Cálculo de valor esperado de la posición y energía cinética

            almacen = exp(-8.0*(4*(N-1)-N)*(4*(N-1)-N)/(1.0*N*N));

            x = 0.0;
            T = 0.0;
            e_x = 0.0;

            //Segunda derivada de psi (para el cálculo de la energía cinética)

            D2psi[0] = 0.0;
            D2psi[N] = 0.0;

            for (int j = 1; j < N; j++) 
            {
                D2psi[j] = (psi[j+1] - 2.0 * psi[j] + psi[j-1]) / (h * h);
            }

            //Cálculo de la energía cinética y posicion y sus errores utilizando integración numérica

            for (int j = 1; j < N; j++) 
            {
                x += j*h*conj(psi[j])*psi[j];

                T += conj(psi[j])*D2psi[j];

                e_x += j*j*h*h*conj(psi[j])*psi[j];
            }

            e_x = sqrt(e_x - x*x);

            fprintf(PELI, "%d;%f;%f\n", t, creal(x), creal(e_x));
            fprintf(CINE, "%f\n", cabs(T));
        }

    }

    printf("El valor de K es: %f\n", 1.0*m_t/1000.0); //1000 iteraciones
    printf("El valor de la probabilidad a la derecha es: %f\n", p_dere_media/1000.0);

    free(psi);
    free(D2psi);
    free(chi);
    free(beta);
    free(alpha);
    free(V);

    fclose(PELI);
    fclose(CINE);

    return 0;
}

//Función para el cálculo de la probabilidad a la derecha
double nomeven_dere(double complex *psi, int N)
{
    double probabilidad = 0.0;

    for (int i = 4*N/5; i < N; i++) 
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
