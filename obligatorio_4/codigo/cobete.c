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

// Declaración de funciones
long double rsimple(long double r_1, long double phi_1, long double p_phi, long double t);
long double phisimple(long double r_1, long double phi_1, long double t);

int main (void)
{
    // Declaración de variables
    long double r_0, r_1, p_r, p_phi, **k, phi_1;
    long double h, v, theta, t;

    FILE *COBETE = fopen("COHETE.dat", "w");
    FILE *CONSERVAS = fopen("H.dat", "w");

    // Asignación de memoria
    k = (long double**)malloc(4*sizeof(long double*));
    for (int i = 0; i < 4; i++)
    {
        k[i] = (long double*)malloc(4*sizeof(long double));
    }

    h = 0.01; // Paso temporal
    t = 0.0; // Tiempo inicial

    // Condiciones iniciales, 0 es la Luna y 1 es el cohete

    r_0 = D;
    r_1 = R_T;

    theta = 3.1415/8; //Ángulo para que llegue

    phi_1 = 0.0;

    v = 11116.0; //Velocidad del cohete, la velocidad de escape es 11200 m/s

    r_0 = r_0/(1.0*D);
    r_1 = r_1/(1.0*D);
    v = v/(1.0*D);

    fprintf(COBETE, "%Lf, %Lf\n", r_0*cosl(0.0), r_0*sinl(0.0));
    fprintf(COBETE, "%Lf, %Lf\n", r_1*cosl(phi_1), r_1*sinl(phi_1));
    fprintf(COBETE, "\n");

    p_r = v*cos(theta); 

    p_phi = r_1*v*sin(theta); 

    // Resolución del sistema de ecuaciones diferenciales

    for (long int i = 0; i < 100000000; i++)
    {
        //Cálculo de k1
        k[0][0] = h*p_r;
        k[0][1] = h*p_phi/(r_1*r_1);
        k[0][2] = h*rsimple(r_1, phi_1, p_phi, i*h);
        k[0][3] = h*phisimple(r_1, phi_1, i*h);

        //Cálculo de k2
        k[1][0] = h*(p_r + k[0][2]/2.0);
        k[1][1] = h*(p_phi + k[0][3]/2.0)/(((r_1 + k[0][0]/2.0)*(r_1 + k[0][0]/2.0)));
        k[1][2] = h*rsimple(r_1 + k[0][2]/2.0, phi_1 + k[0][1]/2.0, p_phi+ k[0][3]/2.0, i*h);
        k[1][3] = h*phisimple(r_1 + k[0][0]/2.0, phi_1 + k[0][1]/2.0, i*h);

        //Cálculo de k3
        k[2][0] = h*(p_r + k[1][2]/2.0);
        k[2][1] = h*(p_phi + k[1][3]/2.0)/(((r_1 + k[1][0]/2.0)*(r_1 + k[1][0]/2.0)));
        k[2][2] = h*rsimple(r_1 + k[1][2]/2.0, phi_1 + k[1][1]/2.0, p_phi + k[1][3]/2.0, i*h);
        k[2][3] = h*phisimple(r_1 + k[1][0]/2.0, phi_1 + k[1][1]/2.0, i*h);


        //Cálculo de k4
        k[3][0] = h*(p_r + k[2][2]); 
        k[3][1] = h*(p_phi + k[2][3])/(((r_1 + k[2][0])*(r_1 + k[2][0])));
        k[3][2] = h*rsimple(r_1 + k[2][2], phi_1 + k[2][1], p_phi + k[2][3], i*h);
        k[3][3] = h*phisimple(r_1 + k[2][0], phi_1 + k[2][1], i*h);

        // Cálculo de las variables en el siguiente paso
        p_phi += (k[0][3] + 2.0*k[1][3] + 2.0*k[2][3] + k[3][3])/6.0;
        r_1 += (k[0][0] + 2.0*k[1][0] + 2.0*k[2][0] + k[3][0])/6.0;
        phi_1 += (k[0][1] + 2.0*k[1][1] + 2.0*k[2][1] + k[3][1])/6.0;
        p_r += (k[0][2] + 2.0*k[1][2] + 2.0*k[2][2] + k[3][2])/6.0;

        if((i%5000)==0)
        {
            fprintf(COBETE, "%Lf, %Lf\n", r_0*cosl(w*t), r_0*sinl(w*t));
            fprintf(COBETE, "%Lf, %Lf\n", r_1*cosl(phi_1), r_1*sinl(phi_1));
            fprintf(COBETE, "\n");

            fprintf(CONSERVAS, "%Lf\n", (p_r*p_r/2.0) + (p_phi*p_phi/(2.0*r_1*r_1)) - ((G/(D*D*D))*((M_T/r_1) + (M_L/(sqrt(1.0 + r_1*r_1 - 2.0*r_1*cos(phi_1 - 1.0*w*t)))))));
        }

        t += h;
    }

    free(k);

    fclose(COBETE);
    fclose(CONSERVAS);

    return 0;
}

//Funciones para calcular el punto r y el punto phi
long double rsimple(long double r_1, long double phi_1, long double p_phi, long double t)
{
    double DELTA, mu,r_2;

    DELTA=G*M_T/(1.0*D*D*D);
    mu=M_L/(1.0*M_T);
    r_2=sqrt(1.0+r_1*r_1-2.0*r_1*cos(phi_1-1.0*w*t));

    return p_phi*p_phi/(r_1*r_1*r_1)-DELTA*(1.0/(r_1*r_1)+mu/(r_2*r_2*r_2)*(r_1-cos(phi_1-1.0*w*t)));
}

long double phisimple(long double r_1, long double phi_1, long double t)
{
    double DELTA, mu,r_2;

    DELTA=G*M_T/(1.0*D*D*D);
    mu=M_L/(1.0*M_T);
    r_2=sqrt(1.0+r_1*r_1-2.0*r_1*cos(phi_1-1.0*w*t));

    return -DELTA*mu*r_1*sin(phi_1-1.0*w*t)/(r_2*r_2*r_2);
}