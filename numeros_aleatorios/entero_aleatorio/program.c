#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

int entero_aleatorio(int dim);
double real_aleatorio();

int main (void){
    srand(time(NULL));
    for (int i = 0; i < 100; i++){
        printf("%d\n", entero_aleatorio(4) + 1);
    }
    /*for (int i = 0; i < 100; i++){
        printf("%f\n", real_aleatorio());
    }
    return 0;*/
}

int entero_aleatorio(int dim)
{
    int n;

    //Genero el número aleatorio con ayuda de rand
    n = ((dim) * rand()/ RAND_MAX);

    return n;
}

double real_aleatorio()
{
    double numero;

    //Genero el número aleatorio con ayuda de rand
    numero = (double)(rand()) / RAND_MAX;

    return numero;
}