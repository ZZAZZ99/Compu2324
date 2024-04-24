#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<omp.h>

void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
short int entero_aleatorio(short int dim);
double magnum(short int **matriz, short int n, short int m);
double energuia(short int **matriz, short int n, short int m);
double real_aleatorio();

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m;
    double Temp, p, E, aux, mj, t;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    Temp=1.0; //Temperatura de la red

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

    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<columnas; j++)
        {
            spiderman[i][j] = 1;
        }
    }
    
    for(t=0; t<1000; t++)
    {
        for(int i=0; i<(filas)*(columnas); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = entero_aleatorio(filas);
            m = entero_aleatorio(columnas);

            //Evalúo p
            E = (2 * spiderman[n][m] * (spiderman[(n+1)%filas][m] + spiderman[(n-1+filas)%filas][m] + spiderman[n][(m+1)%columnas] + spiderman[n][(m-1+columnas)%columnas]));

            aux = exp(-E/Temp);

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
        }

        actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    }

    for(int i = 0; i < filas+1; i++) 
    {
        free(spiderman[i]);
    }

    free(spiderman);

    fclose(DIPOLE);

    return 0;
}

short int entero_aleatorio(short int dim)
{
    short int n;

    //Genero el número aleatorio con ayuda de rand
    n = (short int)((dim * rand()/ RAND_MAX));

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

//Función que calcula la magnetización promedio
double magnum(short int **matriz, short int n, short int m)
{
    double MAG = 0;

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            MAG += matriz[i][j];
        }
    }

    return MAG/(n*m);
}

//Función que calcula la energía media
//Esta cuidado, tienes que arreglarla
double energuia(short int **matriz, short int n, short int m)
{
    double E = 0;

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            E += matriz[i][j] * (matriz[(i+1)%n][j] + matriz[(i-1+n)%n][j] + matriz[i][(j+1)%m] + matriz[i][(j-1+m)%m]);
        }
    }

    return -E/(n*m);
}