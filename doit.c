#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fitsio.h"
#include "readfits.h" // added 8-8-14 LF
#include "magHist.h" // added 11-12-14 LF

int main(int argc, char **argv)
{
    
	int   Nside;
	float Lside,**magmap;
	//void  read_magmap(char *filename, int *Nside, float *Lside, float ***data);
/*
	read_magmap("/Users/lisa/Documents/Summer2014Research/MagnificationMaps/magmaps/A/f0.1i5.fits",&Nside,&Lside,&magmap);
    
    float min_data, max_data;
    find_min_max(magmap[1], Nside * Nside, &min_data, &max_data);
    printf("min_data = %f, max_data = %f\n", min_data, max_data);
	//printf("Nside,Lside= %d %f\n",Nside,Lside);
	//printf("mu[10][12]= %e\n",magmap[10][10]);
	free_matrix(magmap,1,Nside,1,Nside);
*/

    char* input_path = "/Users/lisa/Documents/Summer2014Research/MagnificationMaps/magmaps/A/";
    char* output_path = "/Users/lisa/Documents/Summer2014Research/Histograms/Width1/A_";
    float min = 0.;
    float max = 50.;
    int no_of_bins = 50;
    float bin_width = (max - min)/no_of_bins;
    //float bin_width = 1.1;
    //int no_of_bins = (max - min)/bin_width;
    int no_of_f_values = 5;
    int no_of_i_values = 100;
    
    //printf("no_of_bins = %d\tbin_width = %f\n", no_of_bins, bin_width);
    
    loop_f_val(input_path, output_path, min, max, no_of_bins, bin_width, no_of_f_values, no_of_i_values);
    
    
    input_path = "/Users/lisa/Documents/Summer2014Research/MagnificationMaps/magmaps/D/";
    output_path = "/Users/lisa/Documents/Summer2014Research/Histograms/Width1/D_";

    loop_f_val(input_path, output_path, min, max, no_of_bins, bin_width, no_of_f_values, no_of_i_values);

	return 1;
}

