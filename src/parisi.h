#ifndef PARISI
#define PARISI

#include <stdlib.h>
#include <limits.h>

extern unsigned int *WHEEL;
extern unsigned char IND_RAN;

void pr_srand(unsigned long int seed);
unsigned int pr_rand();

#endif
