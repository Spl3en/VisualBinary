// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-18.51.14
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------


// ------ Class declaration -------
typedef struct _Frame
{
	int data[256][256];

}	Frame;



// --------- Constructors ---------

Frame *
frame_new (void);

Frame *
frame_alloc (void);

// ----------- Methods ------------

void
frame_init (Frame *frame);


// --------- Destructors ----------

void
frame_free (Frame *frame);


