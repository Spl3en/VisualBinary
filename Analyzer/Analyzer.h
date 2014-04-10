// --- Author : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "../Frame/Frame.h"
#include <stdio.h>

// ---------- Defines -------------
#define NB_FRAMES 256


// ------ Class declaration -------
typedef struct _Analyzer
{
	Frame *frames_time;
	Frame *frames_space;
	Frame *frames_fft;

	int maxvalue_time;
	int maxvalue_space;
	int maxvalue_fft;

	FILE *binary;
	long int filesize;

}	Analyzer;


// --------- Constructors ---------
Analyzer *
analyzer_new (char *filename);

Analyzer *
analyzer_alloc (void);


// ----------- Methods ------------

void
analyzer_init (Analyzer *this, char *filename);


// --------- Destructors ----------

void
analyzer_free (Analyzer *analyzer);


