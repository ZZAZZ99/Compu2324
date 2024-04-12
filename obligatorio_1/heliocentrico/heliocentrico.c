#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define c (long double)(1.496e11)  // Unidad astronómica en metros
#define G (long double)(6.674e-11) // Constante de gravitación universal en m^3 kg^-1 s^-2

#define SOL (long double)(1.99e30)  // Masa del sol en kg
#define MER (long double)(3.301e23) // Masa de mercurio en kg
#define VEN (long double)(4.867e24) // Masa de venus en kg
#define TIE (long double)(5.972e24) // Masa de la tierra en kg
#define MAR (long double)(6.417e23) // Masa de marte en kg
#define JUP (long double)(1.898e27) // Masa de jupiter en kg
#define SAT (long double)(5.683e26) // Masa de saturno en kg
#define URA (long double)(8.681e25) // Masa de urano en kg
#define NEP (long double)(1.024e26) // Masa de neptuno en kg
#define PLU (long double)(1.309e22) // Masa de plutón en kg

#define DMER (long double)(57.9e9)  // Distancia de mercurio al sol en metros
#define DVEN (long double)(108.2e9) // Distancia de venus al sol en metros
#define DTIE (long double)(149.6e9) // Distancia de la tierra al sol en metros
#define DMAR (long double)(227.9e9) // Distancia de marte al sol en metros
#define DJUP (long double)(778.3e9) // Distancia de jupiter al sol en metros
#define DSAT (long double)(1.429e12) // Distancia de saturno al sol en metros
#define DURA (long double)(2.871e12) // Distancia de urano al sol en metros
#define DNEP (long double)(4.495e12) // Distancia de neptuno al sol en metros
#define DPLU (long double)(5.906e12) // Distancia de plutón al sol en metros

#define VMER (long double)(47.87e3) // Velocidad de mercurio en m/s
#define VVEN (long double)(35.02e3) // Velocidad de venus en m/s
#define VTIE (long double)(29.78e3) // Velocidad de la tierra en m/s
#define VMAR (long double)(24.077e3) // Velocidad de marte en m/s
#define VJUP (long double)(13.07e3) // Velocidad de jupiter en m/s
#define VSAT (long double)(9.69e3) // Velocidad de saturno en m/s
#define VURA (long double)(6.81e3) // Velocidad de urano en m/s
#define VNEP (long double)(5.43e3) // Velocidad de neptuno en m/s
#define VPLU (long double)(4.74e3) // Velocidad de plutón en m/s

void minish(long double *m, int filas);
void magnify(long double *m, int filas);
void shorten(long double *r, int filas, int columnas);
void prolong(long double *r, int filas, int columnas);
void sleep(long double *t, int filas);
void wakeup(long double *t, int filas);
void motion(long double *a, long double *m, long double *r1, long double *r2, int filas);
void posicion(long double h, long double *r, long double *v, long double *a, int filas);
void watermelon(long double h, long double *w, long double *v, long double *a, int filas);
void sonic(long double h, long double *v, long double *w, long double *a, int filas);
void conservacion(long double *rx, long double *ry, long double *vx, long double *vy, long double *m, long double *ax, long double *ay, FILE *ejemplo, FILE *ejemplo2, int filas, int columnas);

int main()
{
    long double *rx, *ry, *vx, *vy, *wx, *wy, *ax, *ay, *m, *periodo;
    long double h, t;
    int filas, columnas;

    FILE *SALIDA;
    FILE *IBERDROLA;
    FILE *TIOVIVO;

    SALIDA = fopen("salida.txt", "w"); // Fichero de salida
    IBERDROLA = fopen("iberdrola.txt", "w"); // Fichero de salida
    TIOVIVO = fopen("tiovivo.txt", "w"); // Fichero de salida

    h = 0.01;     // Salto, usar 1/100 o 1/1000
    filas = 10;    // 8 + 1 + 1planetas, se usa siempre con < en los bucles
    columnas = 2; // 2 dimensiones, se usa siempre con < en los bucles

    // Asignamos memoria a los arrays para las filas y columnas

    m = (long double *)malloc(filas * sizeof(long double));
    rx = (long double *)malloc(filas * sizeof(long double));
    ry = (long double *)malloc(filas * sizeof(long double));
    vx = (long double *)malloc(filas * sizeof(long double));
    vy = (long double *)malloc(filas * sizeof(long double));
    wx = (long double *)malloc(filas * sizeof(long double));
    wy = (long double *)malloc(filas * sizeof(long double));
    ax = (long double *)malloc(filas * sizeof(long double));
    ay = (long double *)malloc(filas * sizeof(long double));
    periodo = (long double *)malloc(filas * sizeof(long double));

    // Reescalamiento de las masas

    m[0] = SOL;
    m[1] = MER;
    m[2] = VEN;
    m[3] = TIE;
    m[4] = MAR;
    m[5] = JUP;
    m[6] = SAT;
    m[7] = URA;
    m[8] = NEP;
    m[9] = PLU;

    minish(m, filas);

    // Inicializamos las posiciones y las velocidades de los planetas, empiezan en y=0, así como las aceleraciones

    rx[0] = 0;
    rx[1] = DMER;
    rx[2] = DVEN;
    rx[3] = DTIE;
    rx[4] = DMAR;
    rx[5] = DJUP;
    rx[6] = DSAT;
    rx[7] = DURA;
    rx[8] = DNEP;
    rx[9] = DPLU;

    vy[0] = 0;
    vy[1] = VMER;
    vy[2] = VVEN;
    vy[3] = VTIE;
    vy[4] = VMAR;
    vy[5] = VJUP;
    vy[6] = VSAT;
    vy[7] = VURA;
    vy[8] = VNEP;
    vy[9] = VPLU;

    shorten(rx, filas, columnas);

    shorten(vy, filas, columnas);
    wakeup(vy, filas);

    for (int j = 0; j < filas; j++)
    {
        ry[j] = 0;
        ax[j] = 0; 
        ay[j] = 0;
        vx[j] = 0;
        periodo[j] = 0;
    }

    motion(ax, m, rx, ry, filas);
    motion(ay, m, ry, rx, filas);

    // Ejecutamos el algortimo de Verlet

    for (t = 0; t < 1000; t += h)
    {   

        posicion(h, rx, vx, ax, filas);
        posicion(h, ry, vy, ay, filas);

        watermelon(h, wx, vx, ax, filas);
        watermelon(h, wy, vy, ay, filas);

        motion(ax, m, rx, ry, filas);
        motion(ay, m, ry, rx, filas);

        sonic(h, vx, wx, ax, filas);
        sonic(h, vy, wy, ay, filas);

        //verlet(rx, ry, vx, vy, ax, ay, m, h, filas, columnas);
        
        prolong(rx, filas, columnas);
        prolong(ry, filas, columnas);

        for (int i = 0; i < filas; i++)
        {
            fprintf(SALIDA, "%Lf, %Lf\n", rx[i], ry[i]);
        }

        fprintf(SALIDA, "\n");

        for(int i = 1; i < filas; i++)
        {
            if((ry[i] < 0) && (periodo[i]==0))
            {
                periodo[i] = 2*t;
                periodo[i] = periodo[i] / (sqrt((G * SOL) / pow(c, 3)));
                printf("El periodo de %d es %Lfs\n", i, periodo[i]);
            }
        }

        shorten(rx, filas, columnas);
        shorten(ry, filas, columnas);

        conservacion(rx, ry, vx, vy, m, ax, ay, IBERDROLA, TIOVIVO, filas, columnas);
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
    fclose(TIOVIVO);

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

void motion(long double *a, long double *m, long double *r1, long double *r2, int filas)
{
    int i, j;
    long double aux;

    for (i = 0; i < filas; i++) {
        a[i] = 0;
    }

    for (i = 1; i < filas; i++) {
        for (j = 0; j < filas; j++) {
            if (i != j) 
            {
                aux = sqrt(pow(r1[i] - r1[j], 2) + pow(r2[i] - r2[j], 2));
                a[i] -= (m[j] * (r1[i] - r1[j])) / pow(aux, 3);
            }
        }
    }
    return;
}

//Funciones para el algoritmo de Verlet

void posicion(long double h, long double *r, long double *v, long double *a, int filas) 
{
    for (int i = 1; i < filas; i++) 
    {
        r[i] += h * v[i] + 0.5 * h * h * a[i];
    }
}

void watermelon(long double h, long double *w, long double *v, long double *a, int filas) 
{
    for (int i = 1; i < filas; i++) 
    {
        w[i] = v[i] + h * 0.5 * a[i];
    }
}

void sonic(long double h, long double *v, long double *w, long double *a, int filas) 
{
    for (int i = 1; i < filas; i++) 
    {
        v[i] = w[i] + h * 0.5 * a[i];
    }
}



//Algoritmo para la conservación de la energía y el momento angular

void conservacion(long double *rx, long double *ry, long double *vx, long double *vy, long double *m, long double *ax, long double *ay, FILE *ejemplo, FILE *ejemplo2, int filas, int columnas)
{
    long double T[filas], V[filas], L[filas];

    for (int i = 0; i < filas; i++)
    {
        T[i] = 0;
        V[i] = 0;
        L[i] = 0;
    }

    for (int i = 1; i < filas; i++)
    {
        T[i] += 0.5 * m[i] * (pow(vx[i], 2) + pow(vy[i], 2));
        V[i] += - m[i] * sqrt(pow(rx[i], 2) + pow(ry[i], 2)) * sqrt(pow(ax[i], 2) + pow(ay[i], 2));
        L[i] += m[i] * sqrt(pow(rx[i], 2) + pow(ry[i], 2)) * sqrt(pow(vx[i], 2) + pow(vy[i], 2));
    }

    for (int k = 1; k < filas; k++)
    {
        fprintf(ejemplo, "%Lf\n", T[k] + V[k]);
        fprintf(ejemplo2, "%Lf\n", L[k]);
    }
}