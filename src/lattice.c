/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright (C) Bruno Jiménez, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "lattice.h"

lattice create_lattice(unsigned int size, char flag)
{
    lattice a;
    unsigned int i;
    unsigned int size2=size*size;
    a.size=size;
    a.lambda=malloc(size2*sizeof(char));
    a.offset_x=malloc(size*sizeof(int));
    a.offset_y=malloc(size*sizeof(int));
    a.offset2_x=malloc(size*sizeof(int));
    a.offset2_y=malloc(size*sizeof(int));

    switch(flag)
    {
        case '0':
            for(i = 0; i < size2; i++)
            {
                if(((float)pr_rand()/UINT_MAX)<0.5)
                    a.lambda[i]=-1;
                else a.lambda[i]=1;
            }
            break;
        case '1':
            for(i = 0; i < size2; i++)
                a.lambda[i]=1;
            break;
        case '2':
            a.lambda[0]=1;
            for(i = 1; i < size2; i++)
                a.lambda[i]=-a.lambda[i-1];
            break;
    }

    for(i = 0; i < size; i++)
    {
        a.offset_x[i]=1;
        a.offset_y[i]=size;
        a.offset2_x[i]=-1;
        a.offset2_y[i]=-size;
    }
    a.offset_x[size-1]=-(size-1);
    a.offset_y[size-1]=-size2+size;
    a.offset2_x[size-1]=(size-1);
    a.offset2_y[size-1]=size2-size;

    return a;
}

void reset_lattice(lattice *cristal)
{
    unsigned int i;
    unsigned int size2=cristal->size*cristal->size;
    for(i = 0; i < size2; i++)
    {
        if(((float)pr_rand()/UINT_MAX)<0.5)
            cristal->lambda[i]=-1;
        else cristal->lambda[i]=1;
    }
}

void print_lattice(lattice cristal)
{
    unsigned int i;
    unsigned int size2=cristal.size*cristal.size;
    for(i = 0; i < size2; i++)
    {
        printf("%d",cristal.lambda[i]);
        if((i+1)%cristal.size==0) printf("\n");
    }
    printf("\n");
}

double lattice_energy(lattice cristal)
{
    unsigned int i,j;
    long int energy=0;
    unsigned int size2=cristal.size*cristal.size;
    char *now=cristal.lambda;

    for(i = 0; i < cristal.size; i++)
        for(j = 0; j < cristal.size; j++, now++)
            energy+=*now*(*(now+cristal.offset_x[j])+*(now+cristal.offset_y[i]));

    return (double)energy/size2;
}

double lattice_magnetization(lattice cristal)
{
    unsigned int i;
    unsigned int size2=cristal.size*cristal.size;
    long int magnetization=0;

    for(i = 0; i < size2; i++)
        magnetization+=cristal.lambda[i];
    return magnetization/size2;
}

void lattice_data(lattice cristal, double *energy, double *magnet)
{
    unsigned int i, j;
    char *now=cristal.lambda;
    *energy=0;
    *magnet=0;

    for(i = 0; i < cristal.size; i++)
        for(j = 0; j < cristal.size; j++, now++)
        {
            *energy+=*now*(*(now+cristal.offset_x[j])+*(now+cristal.offset_y[i]));
            *magnet+=*now;
        }
    
}

unsigned int evolve_lattice(lattice *cristal, double beta)
{
    unsigned int i,j;
    unsigned int acept;
    char energy_change;
    double change_probability;
    char *now=cristal->lambda;

    acept=0;
    for(i = 0; i < cristal->size; i++)
        for(j = 0; j < cristal->size; j++, now++)
        {
            energy_change=2*(*(now+cristal->offset_x[j])+
                             *(now+cristal->offset_y[i])+
                             *(now+cristal->offset2_x[j])+
                             *(now+cristal->offset2_y[i]));
            change_probability=exp(-beta*energy_change);

            if(change_probability>=1)
            {
                *now=-*now;
                acept++;
            }
            else if(change_probability>(pr_rand()/(float)UINT_MAX))
            {
                *now=-*now;
                acept++;
            }
        }
    return acept;
}
