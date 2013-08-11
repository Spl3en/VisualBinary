#include "Randomizer.h"

float rand_float (float base, float range)
{
	return base + (((float) rand () / (float) RAND_MAX) * range);
}

int rand_int (int base, int range)
{
	return base + (((float) rand () / (float) RAND_MAX) * range);
}
