#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
//#include "gsl_rng.h"

void matriz_aleatoria(short int **matriz, short int n, short int m, short int LOCAL, FILE *f1);
void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
short int entero_aleatorio(short int dim);
double real_aleatorio();

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m, LOCAL;
    double T, p, E, aux, mj, t;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    LOCAL = 0; //LOCAL = 0 --> PC, LOCAL = 1 --> Proteus

    t = 0.0; //Tiempo inicial de la simulación

    T=1.0; //Temperatura de la red

    //Dimensión de nuestra red
    filas = 10; //Filas
    columnas = 10; //Columnas

    //Abro el archivo donde se guardará la matriz
    FILE *DIPOLE;

    DIPOLE = fopen("dipole.txt", "w");

    //Asignamos memoria dinámica a la matriz

    spiderman = (short int **)malloc(filas*sizeof(short int *));
    for(int i=0; i<filas; i++)
    {
        spiderman[i] = (short int *)malloc(columnas*sizeof(short int));
    }

    matriz_aleatoria(spiderman, filas, columnas, LOCAL, DIPOLE);

    //while(t<1000000)
    //{
        for(int i=0; i<filas*columnas; i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = entero_aleatorio(filas);
            m = entero_aleatorio(columnas);

            //Condiciones de contorno periódicas
            if(n == 0)
            {
                spiderman[0][m] = spiderman[filas-1][m];
            }
            else if(n == filas-1)
            {
                spiderman[filas-1][m] = spiderman[0][m];
            }

            if(m == 0)
            {
                spiderman[n][0] = spiderman[n][columnas-1];
            }
            else if(m == columnas-1)
            {
                spiderman[n][columnas-1] = spiderman[n][0];
            }

            //Evalúo p
            E = (2 * spiderman[n][m] * (spiderman[(n+1)][m] + spiderman[(n-1)][m] + spiderman[n][(m+1)] + spiderman[n][(m-1)]));

            aux = exp(-E/T);

            if(aux > 1)
            {
                p = 1;
            }
            else
            {
                p = aux;
            }

            //Genero un número aleatorio entre 0 y 1
            mj = real_aleatorio();

            if(mj < p)
            {
                spiderman[n][m] = -spiderman[n][m];
            }
            actualizar_matriz(spiderman, filas, columnas, DIPOLE);
        }
        t++;

    //}

    for(int i = 0; i < filas; i++) 
    {
        free(spiderman[i]);
    }

    free(spiderman);
    fclose(DIPOLE);

    return 0;
}

//Función que genera una matriz de números aleatorios con rand
void matriz_aleatoria(short int **matriz, short int n, short int m, short int LOCAL, FILE *f1)
{   
    long int aux;

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            if(LOCAL==0){
                aux = rand();
            }
            else{
                //matriz[i][j] = aleatorio_gsl();
            }

            if(aux % 2 == 0)
            {
                matriz[i][j] = 1;
            }
            else
            {
                matriz[i][j] = -1;
            }

            //Muestro el número generado
            fprintf(f1, "%d", matriz[i][j]);

            if (j < m - 1) {
                fprintf(f1, ",");
            }
        }
        
        fprintf(f1, "\n");
    }
    fprintf(f1, "\n");

    return;

}

short int entero_aleatorio(short int dim)
{
    short int n;

    //Genero el número aleatorio con ayuda de rand
    n = (short int)(dim * rand()/ RAND_MAX);

    return n;
}

double real_aleatorio()
{
    double numero;

    //Genero el número aleatorio con ayuda de rand
    numero = (double)(rand()) / RAND_MAX;

    return numero;
}

void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            // Muestro el número generado
            fprintf(f1, "%d", matriz[i][j]);
            
            if (j < m - 1) {
                fprintf(f1, ",");
            }
        }
        
        fprintf(f1, "\n");
    }
    fprintf(f1, "\n");

    return;
}

/*
short int aleatorio_gsl()
{
    short int numero;
    extern gsl_rng *tau;
    
    gsl_rng *tau;
    int semilla=18237247;

    tau=gsl_rng_alloc(gsl_rng_taus);

    gsl_rng_set(tau,semilla);

    numero = gsl_rng_uniform(tau);
    printf("El número aleatorio es: %f\n", numero);

    return (short int)numero;
}
*/
