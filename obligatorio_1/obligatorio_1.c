#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void minish(long double *m, int filas);
void magnify(long double *m, int filas);
void shorten(long double *r, int filas, int columnas);
void prolong(long double *r, int filas, int columnas);
void sleep(long double *t, int filas);
void wakeup(long double *t, int filas);
void motion(long double *rx, long double *ry, long double *ax, long double *ay, long double *m, int filas, int columnas);
void verlet(long double *rx, long double *ry, long double *vx, long double *vy, long double *ax, long double *ay, long double *m, long double h, int filas, int columnas);
void energuia(long double *rx, long double *ry, long double *vx, long double *vy, long double *m, FILE *ejemplo, int filas, int columnas);

#define c (long double)(1.496 * pow(10, 11))  // Unidad astronómica en metros
#define G (long double)(6.674 * pow(10, -11)) // Constante de gravitación universal en m^3 kg^-1 s^-2

#define SOL (long double)(1.99 * pow(10, 30))  // Masa del sol en kg
#define MER (long double)(3.301 * pow(10, 23)) // Masa de mercurio en kg
#define VEN (long double)(4.867 * pow(10, 24)) // Masa de venus en kg
#define TIE (long double)(5.972 * pow(10, 24)) // Masa de la tierra en kg
#define MAR (long double)(6.417 * pow(10, 23)) // Masa de marte en kg

#define DMER (long double)(57.9 * pow(10, 9))  // Distancia de mercurio al sol en metros
#define DVEN (long double)(108.2 * pow(10, 9)) // Distancia de venus al sol en metros
#define DTIE (long double)(149.6 * pow(10, 9)) // Distancia de la tierra al sol en metros
#define DMAR (long double)(227.9 * pow(10, 9)) // Distancia de marte al sol en metros

#define VMER (long double)(47.87 * pow(10, 3)) // Velocidad de mercurio en m/s
#define VVEN (long double)(35.02 * pow(10, 3)) // Velocidad de venus en m/s
#define VTIE (long double)(29.78 * pow(10, 3)) // Velocidad de la tierra en m/s
#define VMAR (long double)(24.077 * pow(10, 3)) // Velocidad de marte en m/s

int main()
{
    long double *rx, *ry, *vx, *vy, *ax, *ay, *m;
    long double h, t;
    int filas, columnas;

    FILE *SALIDA;
    FILE *IBERDROLA;

    SALIDA = fopen("salida.txt", "w"); // Fichero de salida
    IBERDROLA = fopen("iberdrola.txt", "w"); // Fichero de salida

    h = 0.01;     // Salto, usar 1/100 o 1/1000
    filas = 2;    // 4 planetas, se usa siempre con < en los bucles
    columnas = 2; // 2 dimensiones, se usa siempre con < en los bucles

    // Asignamos memoria a los arrays para las filas y columnas

    m = (long double *)malloc(filas * sizeof(long double));
    rx = (long double *)malloc(filas * sizeof(long double));
    ry = (long double *)malloc(filas * sizeof(long double));
    vx = (long double *)malloc(filas * sizeof(long double));
    vy = (long double *)malloc(filas * sizeof(long double));
    ax = (long double *)malloc(filas * sizeof(long double));
    ay = (long double *)malloc(filas * sizeof(long double));

    // Reescalamiento de las masas

    m[0] = MER;
    m[1] = VEN;
    //m[2] = TIE;
    //m[3] = MAR;

    minish(m, filas);

    // Inicializamos las posiciones y las velocidades de los planetas, empiezan en y=0, así como las aceleraciones

    rx[0] = DMER;
    rx[1] = DVEN;
    //rx[2] = DTIE;
    //rx[3] = DMAR;

    vy[0] = VMER;
    vy[1] = VVEN;
    //vy[2] = VTIE;
    //vy[3] = VMAR;

    shorten(rx, filas, columnas);

    shorten(vy, filas, columnas);
    wakeup(vy, filas);

    for (int j = 0; j < filas; j++)
    {
        ry[j] = 0;
        ax[j] = -rx[j]/pow(rx[j], 3); //Añadido el Sol creo
        ay[j] = 0;
        vx[j] = 0;
        // vy[j] = 0;
    }

    motion(rx, ry, ax, ay, m, filas, columnas);

    // Ejecutamos el algortimo de Verlet

    for (t = 0; t < 1000; t += h)
    {
        verlet(rx, ry, vx, vy, ax, ay, m, h, filas, columnas);
        
        prolong(rx, filas, columnas);
        prolong(ry, filas, columnas);

        for (int i = 0; i < filas; i++)
        {
            fprintf(SALIDA, "%Lf, %Lf\n", rx[i], ry[i]);
        }

        fprintf(SALIDA, "\n");

        energuia(rx, ry, vx, vy, m, IBERDROLA, filas, columnas);

        shorten(rx, filas, columnas);
        shorten(ry, filas, columnas);
    }
    
    // Liberamos la memoria de los arrays

    free(m);
    free(rx);
    free(ry);
    free(vx);
    free(vy);
    free(ax);
    free(ay);

    fclose(SALIDA);
    fclose(IBERDROLA);

    return 0;
}

// Funciones para el reescalamiento de la masa

void minish(long double *m, int filas)
{
    for (int i = 0; i < filas; i++)
    {
        m[i] = m[i] / SOL;
    }
}

void magnify(long double *m, int filas)
{
    for (int i = 0; i < filas; i++)
    {
        m[i] = m[i] * SOL;
    }
}

// Funciones para el reescalamiento de la posición

void shorten(long double *r, int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        r[i] = r[i] / c;
    }
}

void prolong(long double *r, int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        r[i] = r[i] * c;
    }
}

// Funciones para el reescalamiento del tiempo

void sleep(long double *t, int filas)
{
    for (int i = 0; i < filas; i++)
    {
        t[i] = t[i] * sqrt((G * SOL) / pow(c, 3));
    }
}

void wakeup(long double *t, int filas)
{
    for (int i = 0; i < filas; i++)
    {
        t[i] = t[i] / sqrt((G * SOL) / pow(c, 3));
    }
}

// Función de la ecuación de movimiento (cálculo de la aceleración) teniendo en cuenta el reescalamiento

void motion(long double *rx, long double *ry, long double *ax, long double *ay, long double *m, int filas, int columnas)
{
    long double aux;
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (i != j)
            {
                aux = sqrt(pow(rx[i] - rx[j], 2) + pow(ry[i] - ry[j], 2));
                ax[i] += -m[j] * (rx[i] - rx[j]) / pow(aux, 3);
                ay[i] += -m[j] * (ry[i] - ry[j]) / pow(aux, 3);
            }
        }
    }
}

//Algoritmo de Verlet

void verlet(long double *rx, long double *ry, long double *vx, long double *vy, long double *ax, long double *ay, long double *m, long double h, int filas, int columnas)
{
    long double wx[filas], wy[filas];

    for (int i = 0; i < filas; i++)
    {
        rx[i] += vx[i] * h + 0.5 * ax[i] * h* h;
        ry[i] += vy[i] * h + 0.5 * ay[i] * h* h;

        wx[i] = vx[i] + 0.5 * ax[i] * h;
        wy[i] = vy[i] + 0.5 * ay[i] * h;

        motion(rx, ry, ax, ay, m, filas, columnas);

        vx[i] = wx[i] + 0.5 * ax[i] * h;
        vy[i] = wy[i] + 0.5 * ay[i] * h;
    }
}

//Algoritmo para la conservación de la energía

void energuia(long double *rx, long double *ry, long double *vx, long double *vy, long double *m, FILE *ejemplo, int filas, int columnas)
{
    long double E[filas];

    for (int i = 0; i < filas; i++)
    {
        E[i] = 0.5 * m[i] * (pow(vx[i], 2) + pow(vy[i], 2));
        for (int j = 0; j < filas; j++)
        {
            if (i != j)
            {
                E[i] += -G * m[i] * m[j] / sqrt(pow(rx[i] - rx[j], 2) + pow(ry[i] - ry[j], 2));
            }
        }
        
    }

    for (int k = 0; k < filas; k++)
    {
        fprintf(ejemplo, "%Lf\n", E[k]);
    }
}