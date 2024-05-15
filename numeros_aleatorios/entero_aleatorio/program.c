#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

int entero_aleatorio(int dim);
double real_aleatorio();
short int entero_aleatorio_custom(short int dim);

int main (void){
    short int filas = 64;
    srand(time(NULL));
    for (int i = 0; i < 1000; i++){
        printf("%d\n", entero_aleatorio_custom(filas-3));
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
short int entero_aleatorio_custom(short int dim)
{
    short int n;

    //Genero el número aleatorio con ayuda de rand
    n = (short int)((dim * rand()/ RAND_MAX)+1);

    return n;
}