#include "Analyzer.h"
#include "../Ztring/Ztring.h"
#include <stdlib.h>
#include <stdio.h>

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
	this->frames_time = calloc(sizeof(Frame *), 256);
	for (int i = 0; i < 256; i++)
		frame_init(&this->frames_time[i]);

	// Pixel
	unsigned char pixel_pos[2];
	unsigned char pixel_last_pos[2];

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset   = 1;
	int c;

	pixel_pos[1] = fgetc (this->binary);
	while ((c = fgetc (this->binary)) != EOF)
	{
		// Cortesi algorithm : shift bytes from the stream
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = c;

		// Frame
		int progress = ((float) cur_offset / filesize) * 256;
		
		// Store in data
		frame_inc (&this->frames_time[progress], pixel_pos[0], pixel_pos[1]);

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		cur_offset++;
	}
}

void
analyze_space (Analyzer *this)
{
	this->frames_space = calloc(sizeof(Frame *), 256);
	for (int i = 0; i < 256; i++)
		frame_init(&this->frames_space[i]);

	// Pixel
	unsigned char pixel_pos[3];
	unsigned char pixel_last_pos[3];

	// File
	rewind(this->binary);
	long int filesize = file_get_size_handler(this->binary);
	this->filesize = filesize;

	// File Offsets
	long int cur_offset = 1;
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
		frame_inc (&this->frames_space[pixel_pos[2]], pixel_pos[0], pixel_pos[1]);

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		pixel_last_pos[2] = pixel_pos[2];
		cur_offset++;
	}
}

void
analyzer_init (Analyzer *this, char *filename)
{
	this->binary = fopen(filename, "rb");

	if (!this->binary)
	{
		printf("File %s cannot be opened.\n", filename);
		return;
	}

	analyze_time(this);
	analyze_space(this);
}

void
analyzer_free (Analyzer *analyzer)
{
	if (analyzer != NULL)
	{
		free (analyzer);
	}
}
