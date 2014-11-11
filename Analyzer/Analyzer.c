#include "Analyzer.h"
#include "fft/fft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static COMPLEX ** frame_to_complex (Frame *frame);
static void complex_to_frame (COMPLEX **complex_2d, Frame *frame, int size);
static void complex2d_free (COMPLEX **c, int size);

Analyzer *
analyzer_new (char *filename)
{
	Analyzer *this;

	if ((this = analyzer_alloc()) == NULL)
		return NULL;

	analyzer_init (this, filename);

	return this;
}

Analyzer *
analyzer_alloc (void)
{
	return calloc (1, sizeof(Analyzer));
}

void
analyze_time (Analyzer *this)
{
	// Pixel
	unsigned char pixel_pos[2];
	unsigned char pixel_last_pos[2];
	memset(pixel_last_pos, 0, sizeof(pixel_last_pos));

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset = 0;
	int c;

	pixel_pos[1] = fgetc (this->binary);
	while ((c = fgetc (this->binary)) != EOF)
	{
		// Cortesi algorithm : shift bytes from the stream
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = c;

		// Frame
		int progress = ((float) cur_offset / filesize) * NB_FRAMES;

		// Store in data
		int value = frame_inc (&this->frames_time[progress], pixel_pos[0], pixel_pos[1]);
		if (this->maxvalue_time < value)
			this->maxvalue_time = value;

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		cur_offset++;
	}
}

/*
void
analyze_fft_time (Analyzer *this)
{
	// Pixel
	unsigned char pixel_pos[2];
	unsigned char pixel_last_pos[2];
	memset(pixel_last_pos, 0, sizeof(pixel_last_pos));

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset = 0;
	int c;

	pixel_pos[1] = fgetc (this->binary);
	while ((c = fgetc (this->binary)) != EOF)
	{
		// Cortesi algorithm : shift bytes from the stream
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = c;

		// Frame
		int progress = ((float) cur_offset / filesize) * NB_FRAMES;

		// Store in data
		int value = frame_inc (&this->frames_fft[progress], pixel_pos[0], pixel_pos[1]);
		if (this->maxvalue_fft < value)
			this->maxvalue_fft = value;

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		cur_offset++;
	}

	// From time, convert with fft
	for (int i = 0; i < NB_FRAMES; i++)
	{
		COMPLEX **complex_2d = frame_to_complex (&this->frames_fft[i]);
		FFT2D (complex_2d, this->frames_fft[i].size, this->frames_fft[i].size, 1);
		complex_to_frame (complex_2d, &this->frames_fft[i], this->frames_fft[i].size);
	}
}
*/


void
analyze_fft_space (Analyzer *this)
{
	// Pixel
	unsigned char pixel_pos[3];
	unsigned char pixel_last_pos[3];
	memset(pixel_last_pos, 0, sizeof(pixel_last_pos));

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset = 0;
	int c;

	pixel_pos[1] = fgetc (this->binary);
	pixel_pos[2] = fgetc (this->binary);
	while ((c = fgetc (this->binary)) != EOF)
	{
		// Cortesi algorithm : shift bytes from the stream
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = pixel_pos[2];
		pixel_pos[2] = c;

		// Store in data
		int value = frame_inc (&this->frames_fft[pixel_pos[2]], pixel_pos[0], pixel_pos[1]);

		if ((this->maxvalue_fft < value)
		&& (pixel_pos[0] != 0 && pixel_pos[1] != 0)) {
			this->maxvalue_fft = value;
		}

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		pixel_last_pos[2] = pixel_pos[2];
		cur_offset++;
	}

	// From space, convert with fft
	for (int i = 0; i < NB_FRAMES; i++)
	{
		COMPLEX **complex_2d = frame_to_complex (&this->frames_fft[i]);
		FFT2D (complex_2d, this->frames_fft[i].size, this->frames_fft[i].size, 1);
		complex_to_frame (complex_2d, &this->frames_fft[i], this->frames_fft[i].size);
		complex2d_free(complex_2d, this->frames_fft[i].size);
	}
}

void
analyze_space (Analyzer *this)
{
	// Pixel
	unsigned char pixel_pos[3];
	unsigned char pixel_last_pos[3];
	memset(pixel_last_pos, 0, sizeof(pixel_last_pos));

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset = 0;
	int c;

	pixel_pos[1] = fgetc (this->binary);
	pixel_pos[2] = fgetc (this->binary);
	while ((c = fgetc (this->binary)) != EOF)
	{
		// Cortesi algorithm : shift bytes from the stream
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = pixel_pos[2];
		pixel_pos[2] = c;

		// Store in data
		int value = frame_inc (&this->frames_space[pixel_pos[2]], pixel_pos[0], pixel_pos[1]);

		if (
			(this->maxvalue_space < value)
		&& (pixel_pos[0] != 0 && pixel_pos[1] != 0))
		{
			this->maxvalue_space = value;
		}

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		pixel_last_pos[2] = pixel_pos[2];
		cur_offset++;
	}
}

#define alloc_frame(x, is_complex)										\
do {	this->frames_##x = calloc(sizeof(Frame), NB_FRAMES);			\
		for (int i = 0; i < NB_FRAMES; i++) { 							\
			frame_init(&this->frames_##x [i], NB_FRAMES, is_complex);	\
		} } while (0);													\

void
analyzer_init (Analyzer *this, char *filename)
{
	this->binary = fopen(filename, "rb");

	if (!this->binary)
	{
		printf("File %s cannot be opened.\n", filename);
		return;
	}

	this->filename = filename;

	alloc_frame(time, FALSE);
	alloc_frame(space, FALSE);
	alloc_frame(fft, TRUE);

	analyze_time(this);
	analyze_space(this);
	analyze_fft_space(this);
}

#undef alloc_frame

void
analyzer_free (Analyzer *analyzer)
{
	if (analyzer != NULL) {
		free (analyzer);
	}
}

void
complex2d_free (COMPLEX **c, int size)
{
	for (int y = 0; y < size; y++) {
		free(c[y]);
	}

	free(c);
}

static COMPLEX ** frame_to_complex (Frame *frame)
{
	COMPLEX **c = malloc(sizeof(COMPLEX *) * frame->size);

	for (int y = 0; y < frame->size; y++)
	{
		c[y] = malloc (sizeof(COMPLEX) * frame->size);

		for (int x = 0; x < frame->size; x++)
		{
			c[y][x].imag = 0;
			c[y][x].real = frame->data_integer[y][x];
		}
	}

	return c;
}

static void complex_to_frame (COMPLEX **c, Frame *frame, int size)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			frame_set_complex (frame, x, y, c[y][x].real, c[y][x].imag);
		}
	}
}
