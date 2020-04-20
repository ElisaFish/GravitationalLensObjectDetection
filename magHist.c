//
//  magHist.c
//  
//
//  Created by Lisa Fishenfeld.
//
//

/*
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "fitsio.h"
#include "readfits.h"
*/

#include "magHist.h"

/////////////// THROUGHOUT, USE MATRIX INDICES 1 TO N /////////////////////////////////

//double min, max, bin_width;
//int no_of_bins = 100;


void find_min_max(float *data, int length, float* min_data, float* max_data)
{
    //int length = *Nside * *Nside;
    int center = (length / 2) + 1;
    int i = 1;
    if (length % 2)                             // Odd number of pixels
    {
        *min_data = data[center];
        *max_data = data[center];
    }
    else if (data[i] <= data[length])     // Even number of pixels
    {
        *min_data = data[i];
        *max_data = data[length];
        i++;
    }
    else
    {
        *min_data = data[length];
        *max_data = data[i];
        i++;
    }
    
    for (int f=length+1-i; i<center; i++,f--)
    {
        if (data[i] <= data[f])
        {
            
            if (data[i] < *min_data)
                *min_data = data[i];
            if (data[f] > *max_data)
                *max_data = data[f];
            

//            *min = fminf(*min, data[i]);
//            *max = fmaxf(*max, data[f]);
        }
        else
        {
            
            if (data[i] > *max_data)
                *max_data = data[i];
            if (data[f] < *min_data)
                *min_data = data[f];
            

//            *max = fmaxf(*max, data[i]);
//            *min = fminf(*min, data[f]);
        }
    }
    
    return;
}

void both_histograms(int* histogram, int* log_histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width)
{
    // Prep for log histogram
    float min_log, max_log;
    
    if (min <=0)
        min_log = log10f (0.1);
    else
        min_log = log10f (min);
    
    max_log = log10f (max);
    
    float bin_width_log = (max_log - min_log) / no_of_bins;
    
    
    for (int i=1,bin=0,bin_log=0; i<=length; i++)
    {
        ////////////// Regular Histogram //////////////////
        if (data[i] < min)          // Underflow bin
        {
            bin = 0;
        }
        else if (data[i] > max)    // Overflow bin
        {
            bin = no_of_bins + 1;
        }
        else if (data[i] == max)   // Maximum value counted in highest regular bin, instead of overflow
            bin = no_of_bins;
        else
        {
            bin = (int) ((data[i] - min) / bin_width) + 1;
        }
        
        histogram[bin]++;
        
        ////////////// Log Histogram //////////////////
        float log_data = log10f (data[i]);
        
        if (log_data < min_log)          // Underflow bin
        {
            bin_log = 0;
        }
        else if (log_data > max_log)    // Overflow bin
        {
            bin_log = no_of_bins + 1;
        }
        else if (log_data == max_log)   // Maximum value counted in highest regular bin, instead of overflow
            bin_log = no_of_bins;
        else
        {
            bin_log = (int) ((log_data - min_log) / bin_width_log) + 1;
        }
        
        log_histogram[bin_log]++;
    }
}

// Must pass: data, length, histogram
// Params: min, max, no_of_bins and/or bin_width
void make_histogram(int* histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width)
{
    //printf("INside make_histogram\n");
    
    //int length = (Nside) * (Nside);
    //float min, max;
    //find_min_max(&data, length, &min, &max);
    //*bin_width = (float) (max - min)/ *no_of_bins;
    //int* histogram[no_of_bins + 2];
    
    //printf("min=%f, max=%f, bin_width=%f\n", min, max, bin_width);
    
    //int bin;
    for (int i=1,bin=0; i<=length; i++)
    {
        //printf("First element of magmap: %f\n", data[i]);
        //printf("i: %d ", i);
        /*
        if (i <= 5)
            printf("%f ", data[i]);
        */
        
        if (data[i] < min)          // Underflow bin
        {
            bin = 0;
            //printf("underflow ");
        }
        else if (data[i] > max)    // Overflow bin
        {
            //printf("overflow ");
            bin = no_of_bins + 1;
        }
        else if (data[i] == max)   // Maximum value counted in highest regular bin, instead of overflow
            bin = no_of_bins;
        else
        {
            bin = (int) ((data[i] - min) / bin_width) + 1;
            //printf("else ");
        }
        
        /*
        if (i <= 5)
            printf("bin = %d\n", bin);
         */
        //printf("make_hist loop after if\n");
        
        histogram[bin]++;
    }
    /*
    printf("\n");
    */
    /*
    for(int j=0; j<=(no_of_bins+1); j++)
    {
        printf("%d ", histogram[j]);
    }
    printf("\n\n");
    */
}

/*
// Log Histogram
void log_histogram(int* log_histogram, float *data, int length, float min, float max, int no_of_bins, float bin_width)
{
    if (min <=0)
        min = 0.1;
    
    for (int i=1,bin=0; i<=length; i++)
    {
        float log_data = log10f (data[i]);
        
        if (log_data < min)          // Underflow bin
        {
            bin = 0;
            //printf("underflow ");
        }
        else if (log_data > max)    // Overflow bin
        {
            //printf("overflow ");
            bin = no_of_bins + 1;
        }
        else if (log_data == max)   // Maximum value counted in highest regular bin, instead of overflow
            bin = no_of_bins;
        else
        {
            bin = (int) ((log_data - min) / bin_width) + 1;
            //printf("else ");
        }
        
        log_histogram[bin]++;
    }
}
 */


// Must pass: directories, min, max, no_of_bins, bin_width
// Params: no_of_f_values, no_of_bins, no_of_i_values, input_path, output_path
void loop_f_val(char* input_path, char* output_path, float min, float max, int no_of_bins, float bin_width, int no_of_f_values, int no_of_i_values)
{
    for (int f=1; f<=no_of_f_values; f++)
    {
        int histogram[no_of_bins + 2];
        int log_histogram[no_of_bins + 2];
        
        for(int q=0; q<(no_of_bins+2); q++)
        {
            histogram[q] = 0;
            log_histogram[q] = 0;
        }
        
        for (int i=1; i<=no_of_i_values; i++)
        {
            //printf("Inside i loop\n");
            
            // make_histogram for "f%.1fi%d.fits", f, i
            
            char filename[100];
            
            //printf("Created filename but didn't initialize\n");
            //int n = sprintf(filename, "f0.%di%d.fits", f, i);
            
            /*
            int n;
            n = sprintf(filename, "%sf0.%di%d.fits", input_path, f, i);
            printf("Initialized filename");
            */
            
            if (sprintf(filename, "%sf0.%di%d.fits", input_path, f, i) >= 0)
            {
                int Nside;
                float Lside, **magmap;
                
                read_magmap(filename, &Nside, &Lside, &magmap);
                
                //make_histogram(&magmap, &Nside, &no_of_bins, &bin_width, &histograms[f-1]);
                
                //make_histogram(histogram, magmap[1], Nside * Nside, min, max, no_of_bins, bin_width);
                both_histograms(histogram, log_histogram, magmap[1], Nside * Nside, min, max, no_of_bins, bin_width);
                
                free_matrix(magmap, 1, Nside, 1, Nside);
            }
            else
            {
                // Error creating filename
                printf("Error creating filename\n");
            }
        }
        
        /*
        // Print histogram
        printf("f=0.%d:\n     <%f: %d\n", f, min, histograms[f-1][0]);
        for (int i=1; i<=no_of_bins; i++)
        {
            printf("     %f-%f: %d\n", min+((i-1) * bin_width), min+(i * bin_width), histograms[f-1][i]);
        }
        printf("     >%f: %d\n", max, histograms[f-1][no_of_bins+1]);
         */
        
        // Prep for log
        float min_log, max_log;
        
        if (min <=0)
            min_log = log10f (0.1);
        else
            min_log = log10f (min);
        
        max_log = log10f (max);
        float bin_width_log = (max_log - min_log) / no_of_bins;
        
        // Save full information to files
        char hist_file_name[100];
        if (sprintf(hist_file_name, "%shist_f0.%d.dat", output_path, f) >=0)
            save_histogram_to_file(hist_file_name, histogram, min, max, no_of_bins, bin_width);
        
        char log_hist_file_name[100];
        if (sprintf(log_hist_file_name, "%shist_log_f0.%d.dat", output_path, f) >=0)
            save_histogram_to_file(log_hist_file_name, log_histogram, min_log, max_log, no_of_bins, bin_width_log);
        
        // Save just enough information to plot
        char plot_file_name[100];
        if (sprintf(plot_file_name, "%splot_f0.%d.dat", output_path, f) >=0)
            save_plot_to_file(plot_file_name, histogram, min, max, no_of_bins, bin_width);
        
        char log_plot_file_name[100];
        if (sprintf(log_plot_file_name, "%splot_log_f0.%d.dat", output_path, f) >=0)
            save_plot_to_file(log_plot_file_name, log_histogram, min_log, max_log, no_of_bins, bin_width_log);
    }
}

/*
// Save histogram to datafile
// Must pass: outfilename, histogram, min, max, no_of_bins, bin_width
void save_histogram_to_file(char* outfilename, int* histogram, float min, float max, int no_of_bins, float bin_width)
{
    ofstream histfile;
    histfile.open (outfilename);
    
    // Print histogram
    histfile << "<" << min << " " << histogram[0] << endl;
    for (int i=1; i<=no_of_bins; i++)
    {
        histfile << min+((i-1) * bin_width) << "-" << min+(i * bin_width) << " " << histogram[i] << endl;
        //printf("%f-%f %d\n", min+((i-1) * bin_width), min+(i * bin_width), histogram[i]);
    }
    histfile << ">" << max << " " << histogram[no_of_bins+1] << endl;
    //printf(">%f %d\n", max, histogram[no_of_bins+1]);
    
    histfile.close;
}
*/

// Save histogram to datafile IN C
void save_histogram_to_file(char* outfilename, int* histogram, float min, float max, int no_of_bins, float bin_width)
{
    //printf("Inside save_histogram\n");
    /*
    for(int j=0; j<=(no_of_bins+1); j++)
    {
        printf("%d ", histogram[j]);
    }
    printf("\n\n");
    */
    
    FILE* histfile;
    histfile = fopen(outfilename, "w");
    
    if (histfile == NULL) {
        printf("Can't open output file %s!\n",
                outfilename);
        exit(1);
    }
    
    // Print histogram
    fprintf(histfile, "<%f %d\n", min, histogram[0]);
    
    for (int i=1; i<=no_of_bins; i++)
    {
        fprintf(histfile, "%f-%f %d\n", min+((i-1) * bin_width), min+(i * bin_width), histogram[i]);
    }
    fprintf(histfile, ">%f %d\n", max, histogram[no_of_bins+1]);
    
    
    fclose(histfile);

}

// Save plot to datafile IN C
void save_plot_to_file(char* outfilename, int* histogram, float min, float max, int no_of_bins, float bin_width)
{
    //printf("Inside save_histogram\n");
    /*
     for(int j=0; j<=(no_of_bins+1); j++)
     {
     printf("%d ", histogram[j]);
     }
     printf("\n\n");
     */
    
    FILE* plotfile;
    plotfile = fopen(outfilename, "w");
    
    if (plotfile == NULL) {
        printf("Can't open output file %s!\n",
               outfilename);
        exit(1);
    }
    
    // Print histogram
    for (int i=1; i<=no_of_bins; i++)
    {
        fprintf(plotfile, "%f %d\n", min+((i - 0.5) * bin_width), histogram[i]);
    }
    
    fclose(plotfile);
    
}

/*
// Input: x, y
// Must pass: data matrix, Nside
// Output: magnification
float bilinear_interpolation(float x, float y, )
{
    // float x,y are reals between 0.5 and (Nside + 0.5)
    
    // Find 4 pixels
    int x_lower, y_lower;
    
    if (x < 1)
        x_lower = 1;
    else if (x >= Nside)
        x_lower = Nside - 1;
    else
        x_lower = (int) floor(x);
    
    if (y < 1)
        y_lower = 1;
    else if (y >= Nside)
        y_lower = Nside - 1;
    else
        y_lower = (int) floor(x);
    
    float t = x - x_lower;
    float u = y - y_lower;
    
    return((*data[x_lower][y_lower] * (1.-t) * (1.-u)) + (*data[x_lower + 1][y_lower] * t * (1.-u)) + (*data[x_lower+1][y_lower+1] * t * u) + (*data[x_lower][y_lower+1] * (1.-t) * u));
}
*/

/*
void position_to_pixel()
{
    float pixel = position * *Nside / *Lside;
}
*/
 
// Read instruction file
// Input: inst_file_name
// output: dir_mag_map, dir_hist
/*
 void setup_from_file()
{
    ifstream instruction_file;
    instruction_file.open(inst_file_name);
    getline(instruction_file, dir_mag_map);
    getline(instruction_file, dir_hist);
    
    while(
    getline(instruction_file, line3);
    getline(instruction_file, line4);
    getline(instruction_file, line5);
    
}
 */

/*
Instruction file:
 directory where mag maps are located
 directory where output histograms will be located
 
 min, max, no_of_bins or bin_width
 
 // sanity checks min < max
 strtok: string parsing
 fcnf
 sscanf
 google C++ string parsing
*/