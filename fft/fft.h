#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef struct _COMPLEX
{
	double real, imag;

} 	COMPLEX;



int FFT(int dir,int m,double *x,double *y);
int FFT2D(COMPLEX **c,int nx,int ny,int dir);
int Powerof2(int nx, int* m, int* twopm);
