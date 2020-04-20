#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fitsio.h"
#include "readfits.h" // added 8-8-14 LF

#define ERR(a) {fprintf(stderr,"ERROR: %s\n",a); exit(0);}

// *Nside is number of pixels per side
// *Lside is length of side in Einstein radii
void read_magmap(char *filename, int *Nside, float *Lside, float ***data)
{
	int i,j,status,anynul;
	long lx,ly;
	float rayamp1,*fitsarr;
	char comment[80];
	fitsfile *fp;
	float *vector(long nl, long nh);
	float **matrix(long nrl, long nrh, long ncl, long nch);
	void free_vector(float *v, long nl, long nh);
	void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);

	status = 0;
	if (fits_open_file(&fp,filename,READONLY,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	fprintf(stdout,"Reading array from FITS file %s\n",filename);
	if (fits_read_key(fp,TLONG,"NAXIS1",&lx,comment,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	if (fits_read_key(fp,TLONG,"NAXIS2",&ly,comment,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	if (lx==ly) {
		*Nside = lx;
	} else {
		ERR("magnification map is not square")
	}
	if (fits_read_key(fp,TFLOAT,"LENGTH",Lside,comment,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	if (fits_read_key(fp,TFLOAT,"RAYAMP1",&rayamp1,comment,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	fitsarr = vector(0,lx*ly-1);
	(*data) = matrix(1,lx,1,ly);
	if (fits_read_img(fp,TFLOAT,1,lx*ly,0,fitsarr,&anynul,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	if (fits_close_file(fp,&status)) {
		fits_report_error(stderr,status);
		exit(status);
	}
	for (i=1;i<=lx;i++) {
		for (j=1;j<=ly;j++) {
			(*data)[i][j] = (float)(fitsarr[lx*(j-1)+i-1]/rayamp1);
		}
	}
	free_vector(fitsarr,0,lx*ly-1);

}



/*************************************************************/
/* utility, from Numerical Recipes                           */
/*************************************************************/

#define NR_END 1
#define FREE_ARG char*

void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

#undef NR_END
#undef FREE_ARG
