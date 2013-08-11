// --- Author : Moreau Cyril - Spl3en
#pragma once

// ---------- Includes ------------
#include "../Frame/Frame.h"

// ---------- Defines -------------


// ------ Class declaration -------
typedef struct _Analyzer
{
	Frame *frames;

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


