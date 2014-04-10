#include "Frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Frame *
frame_new (int size, int is_complex)
{
	Frame *this;

	if ((this = frame_alloc()) == NULL)
		return NULL;

	frame_init(this, size, is_complex);

	return this;
}

Frame *
frame_alloc (void)
{
	return calloc(1, sizeof(Frame));
}

void
frame_init (Frame *this, int size, int is_complex)
{
	this->data = calloc(sizeof(int *), size);

	if (is_complex)
		this->data_img = calloc(sizeof(int *), size);

	for (int i = 0; i < size; i++)
	{
		this->data[i] = calloc(sizeof(int), size);

		if (is_complex)
			this->data_img[i] = calloc(sizeof(int), size);
	}

	this->size = size;
}

void
frame_copy (Frame *dest, Frame *src)
{
	if (dest->size != src->size)
	{
		printf("%s: Frames are not the same size\n", __FUNCTION__);
		return;
	}

	for (int i = 0; i < dest->size; i++)
	{
		memcpy(dest->data[i], src->data[i], sizeof(int *) * dest->size);
	}
}

void
frame_reset (Frame *this)
{
	for (int i = 0; i < this->size; i++)
	{
		memset(this->data[i], 0, sizeof(int *) * this->size);
		memset(this->data_img[i], 0, sizeof(int *) * this->size);
	}
}

void
frame_set_complex (Frame *this, int x, int y, int real, int img)
{
	this->data[y][x] = real;
	this->data_img[y][x] = img;
}

void
frame_get_complex (Frame *this, int x, int y, int *real, int *imag)
{
	*real = this->data[y][x];
	*imag = this->data_img[y][x];
}

int
frame_inc (Frame *this, int x, int y)
{
	this->data[y][x] += 1;
	return this->data[y][x];
}

inline int
frame_get (Frame *this, int x, int y)
{
	return this->data[y][x];
}

void
frame_free (Frame *frame)
{
	if (frame != NULL)
	{
		free (frame);
	}
}
