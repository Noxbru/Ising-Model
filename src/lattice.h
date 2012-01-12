#ifndef LATTICE
#define LATTICE
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
#include "parisi.h"

typedef struct _lattice 
{
    char *lambda;
    int *offset_x;
    int *offset_y;
    int *offset2_x;
    int *offset2_y;
    unsigned int size;
} lattice;

lattice create_lattice(unsigned int size, char flag);
void reset_lattice(lattice *cristal);
void print_lattice(lattice cristal);
unsigned int evolve_lattice(lattice *cristal, double *probs); 
double lattice_energy(lattice cristal);
double lattice_magnetization(lattice cristal);
void lattice_data(lattice cristal, double *energy, double *magnet);

#endif
