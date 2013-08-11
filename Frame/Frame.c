#include "Frame.h"
#include <stdlib.h>
#include <string.h>

Frame *
frame_new ()
{
	Frame *this;

	if ((this = frame_alloc()) == NULL)
		return NULL;

	frame_init(this);

	return this;
}

Frame *
frame_alloc (void)
{
	return calloc(1, sizeof(Frame));
}

void
frame_init (Frame *this)
{
	this->data = calloc(sizeof(int *), FRAME_SIZE);

	for (int i = 0; i < FRAME_SIZE; i++)
	{
		this->data[i] = calloc(sizeof(int), FRAME_SIZE);
	}
}

void
frame_copy (Frame *dest, Frame *src)
{
	for (int i = 0; i < FRAME_SIZE; i++)
	{
		memcpy(dest->data[i], src->data[i], sizeof(int) * FRAME_SIZE);
	}
}

void
frame_reset (Frame *this)
{
	for (int i = 0; i < FRAME_SIZE; i++)
	{
		memset(this->data[i], 0, sizeof(int) * FRAME_SIZE);
	}
}

void
frame_inc (Frame *this, int x, int y)
{
	this->data[y][x] = this->data[y][x] + 1;
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
