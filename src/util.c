#include <raylib.h>

#include "util.h"

void FixAngle(float* a)
{
	while (*a >= PI*2)
		*a -= PI*2;
	while (*a <= 0)
		*a += PI*2;
}

void ModF(float* f)
{
	if(*f < 0)
		*f = -*f;
}
