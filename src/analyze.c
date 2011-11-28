#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    /* Declaration of the variables needed */
    // dummy index and some data
    unsigned int i,j,k;
    unsigned int num_files, num_meas;

    // beta
    double beta;
    
    // energy, magnetization, specific heat and susceptibility
    double       energy,        magnet,         heat,       suscept;
    double       energy_mean,   magnet_mean,    heat_mean,  suscept_mean;
    double       energy_var,    magnet_var,     heat_var,   suscept_var;
    unsigned int *energy_hist,  *magnet_hist;
    double       *energy_ptr,   *magnet_ptr;

    // files needed
    char dir[20];
    char file_name[256];
    FILE *fin;
    FILE *fout_energy;
    FILE *fout_magnet;

    fin=fopen("init.cfg","r");
    fscanf(fin,"%s\n",dir);
    fscanf(fin,"%lf\n",&beta);
    fscanf(fin,"%*u\n");       //Jump over rand_init
    fscanf(fin,"%*c\n");       //Jump over flag
    fscanf(fin,"%u\n",&num_files);
    fscanf(fin,"%u\n",&num_meas);
    fclose(fin);

    energy_ptr=malloc(num_meas*sizeof(double));
    energy_hist=calloc(200,sizeof(unsigned int));

    magnet_ptr=malloc(num_meas*sizeof(double));
    magnet_hist=calloc(200,sizeof(unsigned int));

    fout_energy=fopen("out_energy.dat","w");
    fout_magnet=fopen("out_magnet.dat","w");

    for(i = 0, k = 0; i < num_files; i++)
    {
        sprintf(file_name,"%s%.2lf_%03u",dir,beta,i);
        fin=fopen(file_name,"r");

        fread(energy_ptr,sizeof(double),num_meas,fin);
        fread(magnet_ptr,sizeof(double),num_meas,fin);

        energy_mean=0;
        energy_var=0;
        magnet_mean=0;
        magnet_var=0;
        heat_mean=0;
        heat_var=0;
        suscept_mean=0;
        suscept_var=0;

        for(j = 0; j < num_meas; j++)
        {
            energy=energy_ptr[j];
            energy_mean+=energy;
            energy_var+=energy*energy;
            energy_hist[(int)((energy+1)*100)]++;

            magnet=magnet_ptr[j];
            magnet_mean+=magnet;
            magnet_var+=magnet*magnet;
            magnet_hist[(int)((magnet+1)*100)]++;
        }
        fclose(fin);

        for(j = 0; j < num_meas; j++,k++)
        {
            fprintf(fout_energy,"%u %lf\n",k,energy_ptr[j]);
            fprintf(fout_magnet,"%u %lf\n",k,magnet_ptr[j]);
        }
        printf("%lf ±%lf\t",energy_mean/num_meas,energy_var/num_meas-energy_mean/num_meas/num_meas);
        printf("%lf ±%lf\n",magnet_mean/num_meas,magnet_var/num_meas-magnet_mean/num_meas/num_meas);
    }
    fclose(fout_energy);
    fclose(fout_magnet);


    /* We write the histograms to the corresponding file */
    sprintf(file_name,"hist/energy_%.2lf",beta);
    fout_energy=fopen(file_name,"w");
    sprintf(file_name,"hist/magnet_%.2lf",beta);
    fout_magnet=fopen(file_name,"w");

    for(i = 0; i < 200; i++)
    {
        fprintf(fout_energy,"%.2lf %u\n",(i/100.)-1,energy_hist[i]);
        fprintf(fout_magnet,"%.2lf %f\n",(i/100.)-1,magnet_hist[i]);
    }
    
    return 0;
}
