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
double energia(short int **matriz, short int n, short int m, short int dimension);
double medianaranja(short int **spiderman, short int filas, short int columnas);
double real_aleatorio();

int main(void)
{
    short int **spiderman;
    short int filas, columnas, n, m, contador;
    double Temp, p, E, aux, mj, t, MAG, ENE;

    //Inicializo el valor de la serie de números aleatorios
    srand(time(NULL));

    Temp=5.0; //Temperatura de la red
    MAG = 0.0; //Magnetización inicial de la red
    ENE = 0.0; //Energía inicial de la red

    contador = 0;

    //Dimensión de nuestra red
    filas = 64; //Filas
    columnas = 64; //Columnas
    
    //Abro el archivo donde se guardará la matriz
    FILE *DIPOLE;

    DIPOLE = fopen("dipole.txt", "w");

    //Asignamos memoria dinámica a la matriz

    spiderman = (short int **)malloc((filas+2)*sizeof(short int *));
    for(int i=0; i<filas+2; i++)
    {
        spiderman[i] = (short int *)malloc((columnas+2)*sizeof(short int));
    }

    matriz_aleatoria(spiderman, filas, columnas);

    //Asignamos 1 a la última fila y -1 a la primera fila
    for(int i=0; i<filas; i++)
    {
        spiderman[filas-1][i] = 1;
        spiderman[0][i] = -1;
    }
    for(int j=0; j<columnas; j++)
    {
        //Ponemos una fila de ceros para que si accedemos a la posicion [filas][i] no haya problemas, en el cálculo de la energía (en lugar de un if ya que es más óptimo)
        spiderman[filas][j] = 0;
    }

    actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    
    for(t=0; t<100000; t++)
    {
        for(int i=0; i<(filas)*(columnas); i++)
        {
            //Genero dos posiciones aleatorias para seleccionar un spin aleatorio
            n = entero_aleatorio_custom(filas-3);
            m = entero_aleatorio(columnas-1);

            //Evalúo p
            E = energia(spiderman, n, m, columnas);

            spiderman[n][m] = -spiderman[n][m];

            E = energia(spiderman, n, m, columnas) - E;

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

        //Cálculo de la magnetización promedio y la energía media
        if((int)t%100 == 0)
        {
            //MAG += (magnumsup(spiderman, filas, columnas) + magnuminf(spiderman, filas, columnas))/2.0;
            ENE += medianaranja(spiderman, filas, columnas);

            contador++;
        }

        //actualizar_matriz(spiderman, filas, columnas, DIPOLE);
    }

    //printf("%f", MAG/(1.0*contador));
    printf("%f", ENE/(2.0*filas*contador));

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

//Función utilizada para calcular la energía media de la red
double medianaranja(short int **spiderman, short int filas, short int columnas)
{
    double E;

    E = 0.0;

    for(int i=1; i<filas-1; i++)
        {
            for(int j=0; j<columnas; j++)
            {
                E += spiderman[i][j] * (spiderman[i+1][j] + spiderman[i-1][j] + spiderman[i][(j+1)%columnas] + spiderman[i][(j-1+columnas)%columnas]);
            }
        }

        for(int k=0; k<columnas; k++)
        {
            E += spiderman[filas-1][k] * (spiderman[filas-2][k] + spiderman[filas-1][(k+1)%columnas] + spiderman[filas-1][(k+columnas-1)%columnas]);
            E += spiderman[0][k] * (spiderman[1][k] + spiderman[0][(k+1)%columnas] + spiderman[0][(k+columnas-1)%columnas]);
        }

    return -E/2.0;
}

//Función utilizada para calcular la variación de la energía, solo hacen falta esos componentes debido a que son los únicos que cambian, los demás son constantes
double energia(short int **matriz, short int n, short int m, short int dimension) //cuidado con dimension si no es cuadrada
{
    double E;

    E = 0.0;

    E += matriz[n][m] * (matriz[n+1][m] + matriz[n-1][m] + matriz[n][(m+1)%dimension] + matriz[n][(m-1+dimension)%dimension]); // n, m
    E += matriz[n+1][m] * (matriz[n+2][m] + matriz[n][m] + matriz[n+1][(m+1)%dimension] + matriz[n+1][(m-1+dimension)%dimension]); //n+1, m
    E += matriz[n][(m-1+dimension)%dimension] * (matriz[n+1][(m-1+dimension)%dimension] + matriz[n-1][(m-1+dimension)%dimension] + matriz[n][(m)%dimension] + matriz[n][(m-2+dimension)%dimension]); //n, m-1
    E += matriz[n][(m+1)%dimension] * (matriz[n+1][(m+1)%dimension] + matriz[n-1][(m+1)%dimension] + matriz[n][(m+2)%dimension] + matriz[n][(m+dimension)%dimension]); //n, m+1

    //Se hace porque la posición n-2 se sale únicamente en el caso de n=1
    if(n == 1)
    {
        E += matriz[n-1][m] * (matriz[n][m] + matriz[n-1][(m+1)%dimension] + matriz[n-1][(m-1+dimension)%dimension]); //n-1, m
    }
    else
    {
        E += matriz[n-1][m] * (matriz[n][m] + matriz[n-2][m] + matriz[n-1][(m+1)%dimension] + matriz[n-1][(m-1+dimension)%dimension]); //n-1, m
    }
    
    return -E/2.0;
}