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
	this->data_integer = calloc(sizeof(int *), size);

	if (is_complex) {
		this->data_imag = calloc(sizeof(double *), size);
		this->data_real = calloc(sizeof(double *), size);
	}

	for (int i = 0; i < size; i++)
	{
		this->data_integer[i] = calloc(sizeof(int), size);

		if (is_complex) {
			this->data_imag[i] = calloc(sizeof(double), size);
			this->data_real[i] = calloc(sizeof(double), size);
		}
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
		memcpy(dest->data_integer[i], src->data_integer[i], sizeof(int *) * dest->size);
	}
}

void
frame_reset (Frame *this)
{
	for (int i = 0; i < this->size; i++)
	{
		memset(this->data_integer[i], 0, sizeof(int *) * this->size);
		memset(this->data_imag[i], 0, sizeof(double *) * this->size);
		memset(this->data_real[i], 0, sizeof(double *) * this->size);
	}
}

void
frame_set_complex (Frame *this, int x, int y, double real, double imag)
{
	this->data_real[y][x] = real;
	this->data_imag[y][x] = imag;
}

void
frame_get_complex (Frame *this, int x, int y, double *real, double *imag)
{
	*real = this->data_real[y][x];
	*imag = this->data_imag[y][x];
}

int
frame_inc (Frame *this, int x, int y)
{
	this->data_integer[y][x] += 1;
	return this->data_integer[y][x];
}

inline int
frame_get (Frame *this, int x, int y)
{
	return this->data_integer[y][x];
}

void
frame_free (Frame *frame)
{
	if (frame != NULL)
	{
		free (frame);
	}
}
