#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<omp.h>
//#include <gsl_rng.h>
//#include <gsl/gsl_randist.h>

void matriz_aleatoria(short int **matriz, short int n, short int m, short int LOCAL, FILE *f1);
void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
void copiar_matriz(short int **matriz1, short int **matriz2, short int n, short int m);
short int petizo_gsl(short int dim);
double real_gsl();
int entero_gsl();

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m, LOCAL;
    double T, p, E, aux, mj, t;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    LOCAL = 0; //LOCAL = 0 --> PC, LOCAL = 1 --> Proteus

    //t = 0.0; //Tiempo inicial de la simulación

    T=1.0; //Temperatura de la red

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

    matriz_aleatoria(spiderman, filas, columnas, LOCAL, DIPOLE);

    for(t=0; t<1000; t++)
    {
        for(int i=0; i<(filas)*(columnas); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = petizo_gsl(filas);
            m = petizo_gsl(columnas);

            //Evalúo p
            E = (2 * spiderman[n][m] * (spiderman[(n+1)%filas][m] + spiderman[(n-1+filas)%filas][m] + spiderman[n][(m+1)%columnas] + spiderman[n][(m-1+columnas)%columnas]));

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
            mj = real_gsl();

            if(mj < p)
            {
                spiderman[n][m] = -spiderman[n][m];
            }
        }

        actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    }

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
            aux = aleatorio_gsl();

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

void copiar_matriz(short int **matriz1, short int **matriz2, short int n, short int m)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            matriz2[i][j] = matriz1[i][j];
        }
    }

    return;
}


short int petizo_gsl(short int dim) 
{
    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    // Generar número aleatorio short int entre 0 y dim
    short int random_short = gsl_rng_uniform_int(r, dim);

    gsl_rng_free(r);

    return random_short;
}

double real_gsl() 
{
    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    // Generar número aleatorio entre 0 y 1
    double random_number = gsl_rng_uniform(r);

    gsl_rng_free(r);

    return random_number;
}

int entero_gsl() 
{
    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    // Generar número aleatorio entero
    int random_int = gsl_rng_uniform_int(r, GSL_MAX_INT);

    gsl_rng_free(r);

    return random_int;
}

