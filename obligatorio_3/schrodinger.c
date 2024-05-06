#include<complex.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define PI 3.14159265358979323846

int main (void)
{
    double h, s, lambda, k;
    double *x, *t, *V;
    double complex **psi, **chi, **beta, **b, *alpha;
    int j, n, N, ciclos;

    //Asignamos memoria dinámica

    psi = (double complex **)malloc(sizeof(double complex *) * N);
    chi = (double complex **)malloc(sizeof(double complex *) * N);
    beta = (double complex **)malloc(sizeof(double complex *) * N);
    alpha = (double complex *)malloc(sizeof(double complex) * N);
    b = (double complex **)malloc(sizeof(double complex) * N);
    V = (double *)malloc(sizeof(double) * N);
    x = (double *)malloc(sizeof(double) * N);
    t = (double *)malloc(sizeof(double) * N);

    for(int p=0; p<N+1; p++)
    {
        psi[p] = (double complex *)malloc(sizeof(double complex) * N);
        chi[p] = (double complex *)malloc(sizeof(double complex) * N);
        beta[p] = (double complex *)malloc(sizeof(double complex) * N);
        b[p] = (double complex *)malloc(sizeof(double complex) * N);
    }

    //Definimos constantes
    //Los ciclos los escoges tú
    N = 100;
    n = 100;
    h = 0.01;
    s = 0.01;
    ciclos = 1;
    lambda = 0.3;
    k = 2.0*ciclos*PI/(1.0*N);
    s = 1.0/(4.0*k*k);
    alpha[N-1] = 0;

    for(int q=0; q<n+1; q++)
    {
        beta[N-1][q] = 0;
        chi[0][q] = 0;
        chi[N][q] = 0;
    }

    //Discretizamos x y t

    for (j = 0; j < N+1; j++)
    {
        x[j] = j * h;
    }

    for (int i = 0; i < n+1; i++)
    {
        t[i] = i * s;
    }
    
    //Condiciones de contorno de psi
    psi[0][0] = 0;
    psi[N][0] = 0;

    for(int j=1; j<N; j++)
    {
        psi[j][0] = exp(I*k*j)*exp(-8*(4*j-N)*(4*j-N)/(N*N));
    }

    //Calculamos V

    for (int j=0; j<(2*N/5); j++)
    {
        V[j] = 0;
    }

    for(int j=(2*N/5); j<(3*N/5)+1; j++)
    {
        V[j] = lambda*k*k;
    }

    for(int j=(3*N/5)+1; j<N+1; j++)
    {
        V[j] = 0;
    }

    //Calculamos alfa
    for(int j=N-2; j<-1; j--)
    {
        alpha[j] = -1/((-2 + (2*I/s) - V[j]) + alpha[j+1]);
    }

    //Core del algoritmo
    for(int r=0; r<N; r++)
    {
        for(j=N-2; j>-1; j--)
        {
            b[j+1][r]=4*I*psi[j+1][r]/s;

            beta[j][r] = -alpha[j]*b[j+1][r]-beta[j+1][r];
        }

        for (int j = 1; j < N+1; j++) 
        {

            chi[j][r] = alpha[j - 1] * chi[j - 1][r] + beta[j - 1][r];

        }

        for(j=1; j<N; j++)
        {
            psi[j][r+1] = chi[j][r] - psi[j][r];
        }
    }
    

    return 0;
}