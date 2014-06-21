// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-18.51.14
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------
#define FRAME_SIZE 256

#define frame_local_init() \
	{.data = {[0 ... (FRAME_SIZE - 1)] = {[0 ... (FRAME_SIZE - 1)] = 0}}}

// ------ Class declaration -------
typedef struct _Frame
{
	int **data_integer;
	double **data_real;
	double **data_imag;
	int size;

}	Frame;



// --------- Constructors ---------

Frame *
frame_new (int size, int is_complex);

Frame *
frame_alloc (void);

// ----------- Methods ------------

void
frame_init (Frame *this, int size, int is_complex);

int
frame_get (Frame *this, int x, int y);

int
frame_inc (Frame *this, int x, int y);

void
frame_copy (Frame *dest, Frame *src);

void
frame_reset (Frame *this);

void
frame_set_complex (Frame *this, int x, int y, double real, double img);

void
frame_get_complex (Frame *this, int x, int y, double *real, double *imag);

// --------- Destructors ----------

void
frame_free (Frame *frame);


