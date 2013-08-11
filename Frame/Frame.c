#include "Frame.h"
#include <stdlib.h>

Frame *
frame_new (void)
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
}

void
frame_free (Frame *frame)
{
	if (frame != NULL)
	{
		free (frame);
	}
}
