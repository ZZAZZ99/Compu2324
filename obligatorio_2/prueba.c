#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<limits.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

short int petizo_gsl(short int dim);
double real_gsl();
int entero_gsl();

int main ()
{
    int i;

    printf("100 random short int numbers between 0 and dim:\n");
    for (i = 0; i < 100; i++) {
        short int random_short = petizo_gsl(100);
        printf("%d ", random_short);
    }
    printf("\n");

    printf("100 random double numbers between 0 and 1:\n");
    for (i = 0; i < 100; i++) {
        double random_number = real_gsl();
        printf("%f ", random_number);
    }
    printf("\n");

    printf("100 random integers:\n");
    for (i = 0; i < 100; i++) {
        int random_int = entero_gsl();
        printf("%d ", random_int);
    }
    printf("\n");

    return 0;
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
    int random_int = gsl_rng_uniform_int(r, INT_MAX);

    gsl_rng_free(r);

    return random_int;
}
