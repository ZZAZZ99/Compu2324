//LOCAL = 0 --> PC, LOCAL = 1 --> Proteus
#define LOCAL 0

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gsl_rng.h"

void aleatorio_real();

int main()
{
    extern gsl_rng *tau;

    aleatorio_real();

    return 0;
}

void aleatorio_real()
{
    double numero;
    extern gsl_rng *tau;
    
    if (LOCAL==0)
    {
        //Inicializo el valor de la serie de números aleatorios
        srand(time(NULL));
        //Genero el número aleatorio con ayuda de rand
        numero = (double)(rand()) / RAND_MAX;

        //Muestro el número generado
        printf("El número aleatorio es: %f\n", numero);

    }
    else
    {
        gsl_rng *tau;
        int semilla=18237247;

        tau=gsl_rng_alloc(gsl_rng_taus);

        gsl_rng_set(tau,semilla);

        numero = gsl_rng_uniform(tau);
        printf("El número aleatorio es: %f\n", numero);
        
    }

    return;
}


