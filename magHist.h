//
//  magHist.h
//  
//
//  Created by Lisa Fishenfeld on 11/12/14.
//
//

#ifndef _magHist_h
#define _magHist_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
//#include <iostream>
//#include <fstream.h>
#include "fitsio.h"
#include "readfits.h"

/////////////// THROUGHOUT, USE MATRIX INDICES 1 TO N /////////////////////////////////

void find_min_max(float *data, int length, float* min_data, float* max_data);
void both_histograms(int* histogram, int* log_histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width);
void make_histogram(int* histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width);
//void log_histogram(int* log_histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width);
void loop_f_val(char* input_path, char* output_path, float min, float max, int no_of_bins, float bin_width, int no_of_f_values, int no_of_i_values);
void save_histogram_to_file(char* outfilename, int* histogram, float min, float max, int no_of_bins, float bin_width);
void save_plot_to_file(char* outfilename, int* histogram, float min, float max, int no_of_bins, float bin_width);

#endif
