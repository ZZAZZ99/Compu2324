//LOCAL = 0 --> PC, LOCAL = 1 --> Proteus
#define LOCAL 0
#define DIM 50

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void matriz_aleatoria(short int matriz[][DIM], int n, FILE *archivo);

int main()
{
    FILE *f1; 

    f1 = fopen("matriz.dat", "w");

    short int matriz[DIM][DIM];
    short int n;

    n = DIM;

    matriz_aleatoria(matriz, n, f1);

    fclose(f1);

    return 0;
}

void matriz_aleatoria(short int matriz[][DIM], int n, FILE *f1)
{   
    
    if (LOCAL==0)
    {
        //Inicializo el valor de la serie de números aleatorios
        srand(time(NULL));

        for(int i=0; i<n; i++)
        {
            // fprintf(f1, "(");
            for(int j=0; j<n; j++)
            {
                matriz[i][j] = rand();

                if(matriz[i][j] % 2 == 0)
                {
                    matriz[i][j] = 1;
                }
                else
                {
                    matriz[i][j] = -1;
                }

                //Muestro el número generado
                fprintf(f1, "%d ", matriz[i][j]);
            }
            fprintf(f1, "\n");
        }

    }
    else
    {
        //codigo de gsl
    }

    return;
}