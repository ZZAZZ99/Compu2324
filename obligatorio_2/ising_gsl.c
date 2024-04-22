#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<omp.h>
#include<limits.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

void matriz_aleatoria(short int **matriz, short int n, short int m, FILE *f1, gsl_rng * r);
void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
short int petizo_gsl(gsl_rng * r, short int dim);
double real_gsl(gsl_rng * r);
int entero_gsl(gsl_rng * r);

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m;
    double Tempe, p, E, aux, mj, t;

    //Declaración de cosas de GSL

    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    Tempe=1.0; //Temperatura de la red

    //Dimensión de nuestra red
    filas = 64; //Filas
    columnas = 64; //Columnas

    //Abro el archivo donde se guardará la matriz
    FILE *DIPOLE;

    DIPOLE = fopen("dipole.txt", "w");

    //Asignamos memoria dinámica a la matriz

    spiderman = (short int **)malloc((filas+1)*sizeof(short int *));
    for(int i=0; i<filas+1; i++)
    {
        spiderman[i] = (short int *)malloc((columnas+1)*sizeof(short int));
    }

    matriz_aleatoria(spiderman, filas, columnas, DIPOLE, r);

    for(t=0; t<100000; t++)
    {
        for(int i=0; i<(filas)*(columnas); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = petizo_gsl(r, filas);
            m = petizo_gsl(r, columnas);

            //Evalúo p
            E = (2 * spiderman[n][m] * (spiderman[(n+1)%filas][m] + spiderman[(n-1+filas)%filas][m] + spiderman[n][(m+1)%columnas] + spiderman[n][(m-1+columnas)%columnas]));

            aux = exp(-E/Tempe);

            if(aux > 1)
            {
                p = 1;
            }
            else
            {
                p = aux;
            }

            //Genero un número aleatorio entre 0 y 1
            mj = real_gsl(r);

            if(mj < p)
            {
                spiderman[n][m] = -spiderman[n][m];
            }
        }

        actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    }

    for(int i = 0; i < filas+1; i++) 
    {
        free(spiderman[i]);
    }

    free(spiderman);

    fclose(DIPOLE);

    gsl_rng_free(r);

    return 0;
}

//Función que genera una matriz de números aleatorios con rand
void matriz_aleatoria(short int **matriz, short int n, short int m, FILE *f1, gsl_rng * r)
{   
    int aux;

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            aux = entero_gsl(r);

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

short int petizo_gsl(gsl_rng * r, short int dim) 
{
    // Generar número aleatorio short int entre 0 y dim
    short int random_short = gsl_rng_uniform_int(r, dim);

    return random_short;
}

double real_gsl(gsl_rng * r) 
{
    // Generar número aleatorio entre 0 y 1
    double random_number = gsl_rng_uniform(r);

    return random_number;
}

int entero_gsl(gsl_rng * r) 
{
    // Generar número aleatorio entero
    int random_int = gsl_rng_uniform_int(r, INT_MAX);

    return random_int;
}

