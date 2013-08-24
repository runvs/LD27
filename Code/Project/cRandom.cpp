#include "cRandom.h"


cRandom::cRandom(void)
{
}


cRandom::~cRandom(void)
{
}


int cRandom::GetRandomInt(int min, int max)
{
	if (min == max)
		return min;
	if ( min > max )
	{
		int store = max;
		max = min;
		min = store;
	}
	int diff = max - min;

	return rand() % diff + min;
}


float cRandom::GetRandomFloat(float min, float max)
{
	if (min == max)
		return min;
	if ( min > max )
	{
		float store = max;
		max = min;
		min = store;
	}

	float diff = max - min;

	float r = (float)rand()/(float)RAND_MAX;

	return r * diff + min;
}