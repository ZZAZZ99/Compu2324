#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
//#include "gsl_rng.h"

void matriz_aleatoria(short int **matriz, short int n, short int m, short int LOCAL, FILE *f1);
void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
void copiar_matriz(short int **matriz1, short int **matriz2, short int n, short int m);
short int entero_aleatorio(short int dim);
double real_aleatorio();

int main(void)
{
    short int **spiderman, **spiderman2;
    short int filas, columnas, n, m, LOCAL;
    double T, p, E, aux, mj, t;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    LOCAL = 0; //LOCAL = 0 --> PC, LOCAL = 1 --> Proteus

    t = 0.0; //Tiempo inicial de la simulación

    T=1.0; //Temperatura de la red

    //Dimensión de nuestra red
    filas = 10+2; //Filas
    columnas = 10+2; //Columnas

    //Abro el archivo donde se guardará la matriz
    FILE *DIPOLE;

    DIPOLE = fopen("dipole.txt", "w");

    //Asignamos memoria dinámica a la matriz

    spiderman = (short int **)malloc((filas-1)*sizeof(short int *));
    for(int i=1; i<filas-1; i++)
    {
        spiderman[i] = (short int *)malloc((columnas-1)*sizeof(short int));
    }

    spiderman2 = (short int **)malloc((filas)*sizeof(short int *));
    for(int i=0; i<filas; i++)
    {
        spiderman2[i] = (short int *)malloc((columnas)*sizeof(short int));
    }

    matriz_aleatoria(spiderman, filas-1, columnas-1, LOCAL, DIPOLE);
    copiar_matriz(spiderman, spiderman2, filas-1, columnas-1);

    for(int j=0; j<filas; j++)
    {
        spiderman2[filas-1][j] = spiderman[1][j+1];
        spiderman2[0][j] = spiderman[filas-2][j+1];
        spiderman2[j][columnas-1] = spiderman[j+1][1];
        spiderman2[j][0] = spiderman[j+1][columnas-2];
    }
    

    //while(t<1000000)
    //{
        for(int i=0; i<(filas-2)*(columnas-2); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = entero_aleatorio(filas-2);
            m = entero_aleatorio(columnas-2);

            //Evalúo p
            E = (2 * spiderman2[n][m] * (spiderman2[(n+1)][m] + spiderman2[(n-1)][m] + spiderman2[n][(m+1)] + spiderman2[n][(m-1)]));

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
                spiderman2[n][m] = -spiderman2[n][m];
            }
            actualizar_matriz(spiderman2, filas-1, columnas-1, DIPOLE);
            copiar_matriz(spiderman2, spiderman, filas, columnas);

            for(int j=0; j<filas; j++)
            {
                spiderman2[filas-1][j] = spiderman[1][j+1];
                spiderman2[0][j] = spiderman[filas-2][j+1];
                spiderman2[j][columnas-1] = spiderman[j+1][1];
                spiderman2[j][0] = spiderman[j+1][columnas-2];
            }
    
        }
        t++;

    //}

    for(int i = 1; i < filas-1; i++) 
    {
        free(spiderman[i]);
    }

    free(spiderman);

    for(int i = 0; i < filas; i++) 
    {
        free(spiderman2[i]);
    }

    free(spiderman2);

    fclose(DIPOLE);

    return 0;
}

//Función que genera una matriz de números aleatorios con rand
void matriz_aleatoria(short int **matriz, short int n, short int m, short int LOCAL, FILE *f1)
{   
    long int aux;

    for(int i=1; i<n; i++)
    {
        for(int j=1; j<m; j++)
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
    n = (short int)((dim * rand()/ RAND_MAX) + 1);

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
    for(int i=1; i<n; i++)
    {
        for(int j=1; j<m; j++)
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

void copiar_matriz(short int **matriz1, short int **matriz2, short int n, short int m)
{
    for(int i=1; i<n; i++)
    {
        for(int j=1; j<m; j++)
        {
            matriz2[i][j] = matriz1[i][j];
        }
    }

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
