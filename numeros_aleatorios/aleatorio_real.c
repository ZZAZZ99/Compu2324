//LOCAL = 0 --> PC, LOCAL = 1 --> Proteus
#define LOCAL 0

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void aleatorio_real();

int main()
{
    aleatorio_real();

    return 0;
}

void aleatorio_real()
{
    double numero;
    
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
        //codigo de gsl
    }

    return;
}