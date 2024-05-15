// Algoritmo para el cálculo del aterrizaje de un cohete que despega en la Tierra y aterriza en la Luna.
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>

// Definición de constantes
#define G (long double)(6.674e-11) // Constante de gravitación universal
#define M_T (long double)(5.972e24) // Masa de la Tierra
#define R_T (long double)(6.371e6) // Radio de la Tierra
#define M_L (long double)(7.348e22) // Masa de la Luna
#define R_L (long double)(1.737e6) // Radio de la Luna
#define D (long double)(384.4e6) // Distancia entre la Tierra y la Luna
#define w (long double)(2.662e-6) // Velocidad angular de la Luna
#define m (long double)(1) // Masa del cohete

void shorten(long double *r, int dim);
void prolong(long double *r, int dim);
void PR(long double *p, int dim);
void FALLO(long double *p, int dim);
void openup(long double *p, int dim);
void closedown(long double *p, int dim);

int main (void)
{
    // Declaración de variables
    long double *r, *p_r, *p_phi, *k;
    long double Delta, mu, h, t;
    int dim = 2;

    FILE *COBETE = fopen("COBETE.dat", "w");

    // Asignación de memoria
    k = (long double *)malloc(4*sizeof(long double));
    r = (long double *)malloc(dim*sizeof(long double));
    p_r = (long double *)malloc(dim*sizeof(long double));
    p_phi = (long double *)malloc(dim*sizeof(long double));

    Delta = G*M_T/(D*D*D);
    mu = M_L/M_T;
    h = 0.01;
    t = 0.0;

    // Condiciones iniciales

    r[0] = D + R_L;
    r[1] = R_T;

    shorten(r, dim);

    p_r[0] = 0.0;
    p_r[1] = 0.0;

    p_phi[0] = 0.0;
    p_phi[1] = 0.0;

    // Resolución del sistema de ecuaciones diferenciales

    for (int i = 0; i < 100000; i++)
    {
        // Cálculo de k1
        k[0] = h*p_r[0];
        k[1] = h*p_r[1];
        k[2] = h*(p_phi[0] - 2*w);
        k[3] = h*(p_phi[1] + w);

        // Cálculo de k2
        r[0] += 0.5*k[0];
        r[1] += 0.5*k[1];
        p_r[0] += 0.5*k[2];
        p_r[1] += 0.5*k[3];

        PR(p_r, dim);

        k[0] = h*p_r[0];
        k[1] = h*p_r[1];
        k[2] = h*(p_phi[0] - 2*w);
        k[3] = h*(p_phi[1] + w);

        // Cálculo de k3
        r[0] += 0.5*k[0];
        r[1] += 0.5*k[1];
        p_r[0] += 0.5*k[2];
        p_r[1] += 0.5*k[3];

        PR(p_r, dim);

        k[0] = h*p_r[0];
        k[1] = h*p_r[1];
        k[2] = h*(p_phi[0] - 2*w);
        k[3] = h*(p_phi[1] + w);

        // Cálculo de k4
        r[0] += k[0];
        r[1] += k[1];
        p_r[0] += k[2];
        p_r[1] += k[3];

        PR(p_r, dim);

        k[0] = h*p_r[0];
        k[1] = h*p_r[1];
        k[2] = h*(p_phi[0] - 2*w);
        k[3] = h*(p_phi[1] + w);

        // Cálculo de las variables en el siguiente paso
        r[0] += (k[0]/6.0);
        r[1] += (k[1]/6.0);
        p_r[0] += (k[2]/6.0);
        p_r[1] += (k[3]/6.0);

        PR(p_r, dim);

        t += h;

        /*// Condiciones de aterrizaje
        if (r[0] <= 1.0 && r[1] <= 1.0)
        {
            break;
        }

        // Condiciones de despegue

        if (r[0] >= 1.0 && r[1] >= 1.0)
        {
            break;
        }

        // Condiciones de fallo

        if (r[0] <= 1.0 && r[1] >= 1.0)
        {
            break;
        }

        if (r[0] >= 1.0 && r[1] <= 1.0)
        {
            break;
        }

        // Condiciones de aterrizaje en la Luna

        if (r[0] <= 1.0 && r[1] >= 1.0)
        {
            break;
        }

        if (r[0] >= 1.0 && r[1] <= 1.0)
        {
            break;
        }

        // Condiciones de aterrizaje en la Tierra

        if (r[0] <= 1.0 && r[1] <= 1.0)
        {
            break;
        }

        if (r[0] >= 1.0 && r[1] >= 1.0)
        {
            break;
        }*/

        prolong(r, dim);

        fprintf(COBETE, "%Lf %Lf %Lf %Lf %Lf\n", t, r[0], r[1], p_r[0], p_r[1]);

    }





    free(r);
    free(p_r);
    free(p_phi);

    fclose(COBETE);

    return 0;
}

// Funciones para el reescalamiento de la posición

void shorten(long double *r, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        r[i] = r[i] / D;
    }
}

void prolong(long double *r, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        r[i] = r[i] * D;
    }
}

// Funciones para reescalamiento de p_r y p_phi

void PR(long double *p, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        p[i] = p[i] / (m*D);
    }
}

void FALLO(long double *p, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        p[i] = p[i] * m * D;
    }
}

void openup(long double *p, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        p[i] = p[i] / (m*D*D);
    }
}

void closedown(long double *p, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        p[i] = p[i] * m * D * D;
    }
}