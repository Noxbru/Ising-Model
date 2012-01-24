#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _input_data 
{
    char dir[20];
    unsigned int lat_size;
    double beta;
    unsigned int num_files, num_meas, num_files_jump;
    unsigned int block_size;
} input_data;

input_data init_data();
void print_data();

int main(int argc, const char *argv[])
{
    /* Declaration of the variables needed */
    // dummy index and some data
    unsigned int i,j,k,l;
    unsigned int num_blocks;
    unsigned int num_data;
    unsigned int size2;
    
    // energy, magnetization, specific heat and susceptibility
    // energy is the energy at each moment, same for magnet
    // heat is the specific heat at each block, same for suscept
    // energy_mean is the mean of the values of energy,
    //      same for magnet, heat and suscept
    double       energy,        magnet,         heat,       suscept;
    double       energy_mean,   magnet_mean,    heat_mean,  suscept_mean;
    double       energy_mean2,  magnet_mean2;
    double       energy_mean_block, magnet_mean_block;
    double       heat_mean_block, suscept_mean_block;
    double       energy_mean2_block, magnet_mean2_block;
    double       heat_mean2_block, suscept_mean2_block;
    double       energy_var,    magnet_var;
    unsigned int *energy_hist,  *magnet_hist;
    double       *energy_ptr,   *magnet_ptr;

    // files needed
    char file_name[256];
    FILE *fin;

    input_data input;
    input=init_data();

    //for(input.block_size=1;input.block_size<400;input.block_size++)
    //{
    size2=input.lat_size*input.lat_size;
    num_data=input.num_files*input.num_meas;
    num_blocks=num_data/input.block_size;

    energy_ptr=malloc(num_data*sizeof(double));
    energy_hist=calloc(200,sizeof(unsigned int));

    magnet_ptr=malloc(num_data*sizeof(double));
    magnet_hist=calloc(200,sizeof(unsigned int));

    num_data=(input.num_files-input.num_files_jump)*input.num_meas;
    num_blocks=num_data/input.block_size;
    num_data=num_blocks*input.block_size; // If we don't recalculate num_data, we can have problems dividing

    //===================================
    //for(input.beta=0.1;input.beta<2.00;input.beta+=0.1)
    for(input.beta=0.2;input.beta<0.61;input.beta+=0.01)
    {
    for(i = 0; i < 200; i++)
    {
        energy_hist[i]=0;
        magnet_hist[i]=0;
    }
    
    for(i = input.num_files_jump; i < input.num_files; i++)
    {
        /*printf("%s\n",input.dir);
        printf("%u\n",input.lat_size);
        printf("%.2lf\n",input.beta);  este peta cuando i = input.num_files - input.num_files_jump 
        printf("%03u\n",i);
        printf("%s%u/%.2lf/%03u\n",input.dir,input.lat_size,input.beta,i);*/
        sprintf(file_name,"%s%u/%.2lf/%03u",input.dir,input.lat_size,input.beta,i);
        fin=fopen(file_name,"r");
        fread(energy_ptr+(i-input.num_files_jump)*input.num_meas,sizeof(double),input.num_meas,fin);
        fread(magnet_ptr+(i-input.num_files_jump)*input.num_meas,sizeof(double),input.num_meas,fin);
        fclose(fin);
    }

    energy_mean=0;
    energy_mean2=0;
    energy_var=0;
    magnet_mean=0;
    magnet_mean2=0;
    magnet_var=0;
    heat_mean=0;
    suscept_mean=0;

    heat_mean_block=0;
    heat_mean2_block=0;
    suscept_mean_block=0;
    suscept_mean2_block=0;

    for(i = 0, k = 0; i < num_blocks; i++)
    {
        
        energy_mean_block=0;
        energy_mean2_block=0;
        magnet_mean_block=0;
        magnet_mean2_block=0;
        

        for(j = 0; j < input.block_size; j++, k++)
        {
            energy=energy_ptr[k];
            energy_mean_block+=energy;
            energy_mean2_block+=energy*energy;
            energy_hist[(int)((energy+1)*100)]++;

            magnet=fabs(magnet_ptr[k]);
            magnet_mean_block+=magnet;
            magnet_mean2_block+=magnet*magnet;
            magnet_hist[(int)((magnet_ptr[k]+1)*100)]++;
        }
        
        energy_mean+=energy_mean_block;
        energy_mean2+=energy_mean2_block;
        magnet_mean+=magnet_mean_block;
        magnet_mean2+=magnet_mean2_block;

        energy_mean_block/=input.block_size;
        energy_mean2_block/=input.block_size;
        magnet_mean_block/=input.block_size;
        magnet_mean2_block/=input.block_size;

        energy_var+=energy_mean_block*energy_mean_block;
        magnet_var+=magnet_mean_block*magnet_mean_block;

        heat=2*size2*(energy_mean2_block-energy_mean_block*energy_mean_block);
        suscept=size2*(magnet_mean2_block-magnet_mean_block*magnet_mean_block);

        heat_mean_block+=heat;
        heat_mean2_block+=heat*heat;
        suscept_mean_block+=suscept;
        suscept_mean2_block+=suscept*suscept;
    }

    energy_mean/=num_data;
    energy_mean2/=num_data;
    energy_var/=num_blocks;

    magnet_mean/=num_data;
    magnet_mean2/=num_data;
    magnet_var/=num_blocks;

    heat_mean=2*size2*(energy_mean2-energy_mean*energy_mean);
    heat_mean_block/=num_blocks;
    heat_mean2_block/=num_blocks;
    suscept_mean=size2*(magnet_mean2-magnet_mean*magnet_mean);
    suscept_mean_block/=num_blocks;
    suscept_mean2_block/=num_blocks;

/*
    printf(" <E>  = %g ± %g\n",energy_mean,sqrt((energy_var-energy_mean*energy_mean)/num_blocks));
    printf("<|m|> = %g ± %g\n",magnet_mean,sqrt((magnet_var-magnet_mean*magnet_mean)/num_blocks));
    printf(" <Cv> = %g ± %g\n",heat_mean,sqrt((heat_mean2_block-heat_mean_block*heat_mean_block)/num_blocks));
    printf(" <X>  = %g ± %g\n",suscept_mean,sqrt(suscept_mean2_block-suscept_mean_block*suscept_mean_block)/sqrt(num_blocks));
*/
    printf("%lf ",input.beta);
    printf(" %g %g\t",energy_mean,sqrt((energy_var-energy_mean*energy_mean)/num_blocks));
    printf(" %g %g\t",magnet_mean,sqrt((magnet_var-magnet_mean*magnet_mean)/num_blocks));
    printf(" %g %g\t",heat_mean,sqrt((heat_mean2_block-heat_mean_block*heat_mean_block)/num_blocks));
    printf(" %g %g\n",suscept_mean,sqrt(suscept_mean2_block-suscept_mean_block*suscept_mean_block)/sqrt(num_blocks));

    /*printf("%u %g %g %g %g\n",input.block_size,sqrt(energy_var-energy_mean*energy_mean)/sqrt(num_blocks),sqrt(magnet_var-magnet_mean*magnet_mean)/sqrt(num_blocks),
                              sqrt(heat_mean2_block-heat_mean_block*heat_mean_block)/sqrt(num_blocks),
                              sqrt(suscept_mean2_block-suscept_mean_block*suscept_mean_block)/sqrt(num_blocks));
    }*/

    print_data(input,energy_ptr,energy_hist,magnet_ptr,magnet_hist);
    }

    return 0;
}

input_data init_data()
{
    input_data dat;
    FILE *fin;
    fin=fopen("init.cfg","r");
    fscanf(fin,"%s\n",dat.dir);
    fscanf(fin,"%u\n",&dat.lat_size);
    fscanf(fin,"%lf\n",&dat.beta);
    fscanf(fin,"%*u\n");                //Jump over rand_init
    fscanf(fin,"%*c\n");                //Jump over flag
    fscanf(fin,"%u\n",&dat.num_files);
    fscanf(fin,"%u\n",&dat.num_meas);
    fscanf(fin,"%*u\n");                //Jump over num_sweeps
    fscanf(fin,"%u\n",&dat.num_files_jump);
    fscanf(fin,"%u\n",&dat.block_size);
    fclose(fin);
    return dat;
}

void print_data(input_data input, double *energy_dat, unsigned int *energy_hist,
                                  double *magnet_dat, unsigned int *magnet_hist)
{
    char file_name[256];
    unsigned int i;
    unsigned int num_data;
    FILE *fout_energy, *fout_magnet;

    num_data=input.num_files*input.num_meas;

    sprintf(file_name,"%s%u/%.2lf/out_energy.dat",input.dir,input.lat_size,input.beta);
    fout_energy=fopen(file_name,"w");
    sprintf(file_name,"%s%u/%.2lf/out_magnet.dat",input.dir,input.lat_size,input.beta);
    fout_magnet=fopen(file_name,"w");

    for(i = 0; i < num_data; i++)
    {
        fprintf(fout_energy,"%u %lf\n",i,energy_dat[i]);
        fprintf(fout_magnet,"%u %lf\n",i,magnet_dat[i]);
    }

    fclose(fout_energy);
    fclose(fout_magnet);


    /* We write the histograms to the corresponding file */
    sprintf(file_name,"%s%u/%.2lf/hist_energy",input.dir,input.lat_size,input.beta);
    fout_energy=fopen(file_name,"w");
    sprintf(file_name,"%s%u/%.2lf/hist_magnet",input.dir,input.lat_size,input.beta);
    fout_magnet=fopen(file_name,"w");

    for(i = 0; i < 200; i++)
    {
        fprintf(fout_energy,"%.2lf %g\n",(i/100.)-1,(double)energy_hist[i]/num_data);
        fprintf(fout_magnet,"%.2lf %g\n",(i/100.)-1,(double)magnet_hist[i]/num_data);
    }

    fclose(fout_energy);
    fclose(fout_magnet);
}
