#include <stdlib.h>
#include <limits.h>
#include "parisi.h"

unsigned int *WHEEL;
unsigned char IND_RAN=0;

void pr_srand(unsigned long int seed)
{
    srand(seed);
    WHEEL = malloc(256*sizeof(unsigned int));
    unsigned char i;
    for(i = 0; i != 255; i++)
        WHEEL[i]=rand();
    WHEEL[255]=rand();
}

/* devuelve un número entre 0 y UINT_MAX */
unsigned int pr_rand()
{
    unsigned int r;

    //Chorradas para sacar el numerito que queremos
    WHEEL[IND_RAN]=WHEEL[(unsigned char)(IND_RAN-24)]+WHEEL[(unsigned char)(IND_RAN-55)];
    r=(WHEEL[IND_RAN]^(WHEEL[(unsigned char)(IND_RAN-61)]));
    IND_RAN++; //nos movemos en la rueda
    return r;
}
