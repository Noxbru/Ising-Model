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
#include <time.h>
#include <unistd.h>
#include "lattice.h"
#include "parisi.h"

typedef struct _input_data 
{
    char dir[20];
    double beta;
    unsigned int rand_init;
    char flag;
    unsigned int num_files, num_meas, num_sweeps;
} input_data;

input_data init_data();

typedef struct _out_data 
{
    char file_name[256];
    double *energy_ptr, *magnet_ptr;
} output_data;

void out_data();

int main(int argc, const char *argv[])
{
    unsigned int i,j,k;
    unsigned int acept;

    lattice a;
    unsigned int lat_size=16;

    //double energy, magnet;

    input_data input;
    output_data output;
    char command[30];

    input=init_data();

    if(input.rand_init==0)
        pr_srand(time(NULL));
    else
        pr_srand(input.rand_init);

    a=create_lattice(lat_size,input.flag);
    print_lattice(a);

    output.energy_ptr=malloc(input.num_meas*sizeof(double));
    output.magnet_ptr=malloc(input.num_meas*sizeof(double));

    sprintf(command,"mkdir -p %s%u/%.2lf/",input.dir,lat_size,input.beta);
    system(command);

    for(i = 0; i < input.num_files; i++)
    {
        acept=0;
        for(j = 0; j < input.num_meas; j++)
        {
            for(k = 0; k < input.num_sweeps; k++)
                acept+=evolve_lattice(&a,input.beta);

            //energy=lattice_energy(a);
            //magnet=lattice_magnetization(a);
            //output.energy_ptr[j]=energy;
            //output.magnet_ptr[j]=magnet;
            lattice_data(a,&output.energy_ptr[j],&output.magnet_ptr[j]);
        }
        printf("%lf\n",(double)acept/input.num_meas/input.num_sweeps/256);
        out_data(input,lat_size,output,i);
    }

    return 0;
}

input_data init_data()
{
    input_data dat;
    FILE *fin;
    fin=fopen("init.cfg","r");
    fscanf(fin,"%s\n",dat.dir);
    fscanf(fin,"%lf\n",&dat.beta);
    fscanf(fin,"%u\n",&dat.rand_init);
    fscanf(fin,"%c\n",&dat.flag);
    fscanf(fin,"%u\n",&dat.num_files);
    fscanf(fin,"%u\n",&dat.num_meas);
    fscanf(fin,"%u\n",&dat.num_sweeps);
    fclose(fin);
    return dat;
}

void out_data(input_data inp, unsigned int lat_size, output_data out, unsigned int i)
{
    FILE *fout;
    sprintf(out.file_name,"%s/%u/%.2lf/%03u",inp.dir,lat_size,inp.beta,i);
    fout=fopen(out.file_name,"wt");
    fwrite(out.energy_ptr,sizeof(double),inp.num_meas,fout);
    fwrite(out.magnet_ptr,sizeof(double),inp.num_meas,fout);
    fclose(fout);
}
