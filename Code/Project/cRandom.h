#pragma once

#include <stdlib.h>     /* srand, rand */

class cRandom
{
private:
	cRandom(void);
public:
	~cRandom(void);

	static int GetRandomInt(int min, int max);
	static float GetRandomFloat(float min, float max);
	
};

