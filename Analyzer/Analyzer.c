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
analyzer_init (Analyzer *this, char *filename)
{
	FILE *bin = fopen(filename, "rb");

	if (!bin)
	{
		printf("File %s cannot be opened.\n", filename);
		return;
	}

	// Analyzer frames
	this->frames = calloc(sizeof(Frame *), 256);
	for (int i = 0; i < 256; i++)
	{
		frame_init(&this->frames[i]);
	}

	// Frame
	Frame frame;
	frame_init(&frame);

	unsigned int points_in_frame = 1;

	// Pixel
	unsigned char pixel_pos[2];
	unsigned char pixel_last_pos[2];
	// unsigned char pixel_color;

	// File
	long int filesize = file_get_size_handler(bin);

	// File Offsets
	long int cur_offset   = 1;
	long int start_offset = 0;
	int last_progress     = 0;

	int c;

	pixel_pos[1] = fgetc (bin);
	while ((c = fgetc (bin)) != EOF)
	{
		// Cortesi algorithm
		pixel_pos[0] = pixel_pos[1];
		pixel_pos[1] = c;

		// Color computation
		// pixel_color = ((float) cur_offset / filesize) * 256;

		// Store in data
		frame_inc (&frame, pixel_pos[0], pixel_pos[1]);

		// Draw in bitmap
		//rgb_pixel_t pixel = {256 - pixel_color, 128 - (pixel_color/2), pixel_color, 0};
		//bmp_set_pixel (binary.bmp, pixel_pos[0], pixel_pos[1], pixel);

		// Get the distance from the last point
		/// float distance = get_euclidian_distance (pixel_pos, pixel_last_pos);

		int progress = ((float) cur_offset / filesize) * 256;
		if (progress != last_progress)
		{
			// Save
			frame_copy(&this->frames[last_progress], &frame);
			frame_reset(&frame);
			points_in_frame = 0;
			last_progress = progress;
		}

		// Final computation before next iteration
		pixel_last_pos[0] = pixel_pos[0];
		pixel_last_pos[1] = pixel_pos[1];
		cur_offset++;
		points_in_frame++;
	}

	if (last_progress == 255)
		frame_copy(&this->frames[255], &frame);
}

void
analyzer_free (Analyzer *analyzer)
{
	if (analyzer != NULL)
	{
		free (analyzer);
	}
}
