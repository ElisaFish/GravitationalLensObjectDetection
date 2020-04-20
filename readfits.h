//
//  readfits.h
//  
//
//  Created by Lisa Fishenfeld on 8/8/14.
//
//

#ifndef _readfits_h
#define _readfits_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fitsio.h"

#define ERR(a) {fprintf(stderr,"ERROR: %s\n",a); exit(0);}

// *Nside is number of pixels per side
// *Lside is length of side in Einstein radii
void read_magmap(char *filename, int *Nside, float *Lside, float ***data);

/*************************************************************/
/* utility, from Numerical Recipes                           */
/*************************************************************/

#define NR_END 1
#define FREE_ARG char*

void nrerror(char error_text[]);
/* Numerical Recipes standard error handler */

float *vector(long nl, long nh);
/* allocate a float vector with subscript range v[nl..nh] */

float **matrix(long nrl, long nrh, long ncl, long nch);
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */

void free_vector(float *v, long nl, long nh);
/* free a float vector allocated with vector() */

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);
/* free a float matrix allocated by matrix() */

#undef NR_END
#undef FREE_ARG

#endif
