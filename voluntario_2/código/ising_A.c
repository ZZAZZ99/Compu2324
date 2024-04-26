#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<omp.h>

void matriz_aleatoria(short int **matriz, short int n, short int m);
void actualizar_matriz(short int **matriz, short int n, short int m, FILE *f1);
short int entero_aleatorio(short int dim);
short int entero_aleatorio_custom(short int dim);
double magnumsup(short int **matriz, short int n, short int m);
double magnuminf(short int **matriz, short int n, short int m);
double energia(short int **matriz, short int n, short int m);
double real_aleatorio();

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m, contador;
    double Temp, p, E, aux, mj, t, MAG;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    Temp=1; //Temperatura de la red
    MAG = 0.0; //Magnetización inicial de la red

    contador = 0;

    //Dimensión de nuestra red
    filas = 32; //Filas
    columnas = 32; //Columnas
    
    //Abro el archivo donde se guardará la matriz
    FILE *DIPOLE;

    DIPOLE = fopen("dipole.txt", "w");

    //Asignamos memoria dinámica a la matriz

    spiderman = (short int **)malloc((filas+1)*sizeof(short int *));
    for(int i=0; i<filas+1; i++)
    {
        spiderman[i] = (short int *)malloc((columnas+1)*sizeof(short int));
    }

    matriz_aleatoria(spiderman, filas, columnas);

    //Asignamos 1 a la última fila y -1 a la primera fila
    for(int i=0; i<filas; i++)
    {
        spiderman[filas-1][i] = 1;
        spiderman[0][i] = -1;
    }

    actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    
    for(t=0; t<10000; t++)
    {
        for(int i=0; i<(filas)*(columnas); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = entero_aleatorio_custom(filas-3);
            m = entero_aleatorio(columnas);

            //Evalúo p
            //E = (2 * spiderman[n][m] * (spiderman[(n+1)%filas][m] + spiderman[(n-1+filas)%filas][m] + spiderman[n][(m+1)%columnas] + spiderman[n][(m-1+columnas)%columnas]));
            E = energia(spiderman, filas, columnas);

            spiderman[n][m] = -spiderman[n][m];

            E = energia(spiderman, filas, columnas) - E;

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

            if(mj > p)
            {
                spiderman[n][m] = -spiderman[n][m];
            }
        }

        //Cálculo de la magnetización promedio
        if((int)t%100 == 0)
        {
            MAG += (magnumsup(spiderman, filas, columnas) + magnuminf(spiderman, filas, columnas))/2.0;
            contador++;
        }

        actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    }

    printf("%f", MAG/1.0*contador);

    for(int i = 0; i < filas+1; i++) 
    {
        free(spiderman[i]);
    }

    free(spiderman);

    fclose(DIPOLE);

    return 0;
}

//Función que genera una matriz de números aleatorios con rand
void matriz_aleatoria(short int **matriz, short int n, short int m)
{   
    long int aux;

    for(int i=1; i<n-1; i++)
    {
        for(int j=0; j<m; j++)
        {
            aux = rand();

            if(aux % 2 == 0)
            {
                matriz[i][j] = 1;
            }
            else
            {
                matriz[i][j] = -1;
            }
        }
    }

    return;

}

short int entero_aleatorio(short int dim)
{
    short int n;

    //Genero el número aleatorio con ayuda de rand
    n = (short int)((dim * rand()/ RAND_MAX));

    return n;
}

short int entero_aleatorio_custom(short int dim)
{
    short int n;

    //Genero el número aleatorio con ayuda de rand
    n = (short int)((dim * rand()/ RAND_MAX)+1);

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

double magnumsup(short int **matriz, short int n, short int m)
{
    double MAG;

    MAG = 0.0;

    for(int i=0; i<n/2; i++)
    {
        for(int j=0; j<m; j++)
        {
            MAG += matriz[i][j];
        }
    }

    return fabs(MAG)/(n*m);
}

double magnuminf(short int **matriz, short int n, short int m)
{
    double MAG;

    MAG = 0.0;

    for(int i=n/2; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            MAG += matriz[i][j];
        }
    }

    return fabs(MAG)/(n*m);
}

double energia(short int **matriz, short int n, short int m)
{
    double E;

    E = 0.0;

    for(int i=1; i<n-1; i++)
    {
        for(int j=0; j<m; j++)
        {
            E += matriz[i][j] * (matriz[i+1][j] + matriz[i-1][j] + matriz[i][(j+1)%m] + matriz[i][(j-1+m)%m]);
        }
    }

    return -E/2.0;
}