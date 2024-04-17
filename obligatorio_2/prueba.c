#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int main ()
{
    int dimension = 100;
    printf("Dimension: %d\n", dimension%100);
    printf("Dimension: %d\n", dimension%200);
    printf("Dimension: %d\n", dimension%99);
    printf("Dimension: %d\n", dimension%101);
    printf("Dimension: %d\n", dimension%102);
    printf("Dimension: %d\n", (1+103)%103);
    printf("Dimension: %d\n", (-1+10)%10);
}