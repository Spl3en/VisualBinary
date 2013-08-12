// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-18.51.14
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------
#define FRAME_SIZE 256

#define frame_local_init() \
	{.data = {[0 ... 255] = {[0 ... 255] = 0}}}

// ------ Class declaration -------
typedef struct _Frame
{
	int **data;

}	Frame;



// --------- Constructors ---------

Frame *
frame_new (void);

Frame *
frame_alloc (void);

// ----------- Methods ------------

void
frame_init (Frame *this);

int
frame_get (Frame *this, int x, int y);

int
frame_inc (Frame *this, int x, int y);

void
frame_copy (Frame *dest, Frame *src);

void
frame_reset (Frame *this);


// --------- Destructors ----------

void
frame_free (Frame *frame);


