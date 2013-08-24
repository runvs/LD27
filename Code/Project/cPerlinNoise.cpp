#include <cmath>

#include "cPerlinNoise.h"

cPerlinNoise::cPerlinNoise(int iOctaves, int iPersistence)
{
	m_iOctaves     = iOctaves;
	m_iPersistence = iPersistence;
}

float cPerlinNoise::Noise(int iX, int iY, int iFunction)
{
	int n = iX + iY * 57;
	n = (n << 13) ^ n;

	// Don't ask.
	switch(iFunction)
	{
		case 0:
			return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
		case 1:
			return ( 1.0 - ( (n * (n * n * 15733 + 789227) + 1376312627) & 0x7fffffff) / 1073741827.0);
		case 2:
			return ( 1.0 - ( (n * (n * n * 15737 + 789251) + 1376312629) & 0x7fffffff) / 1073741831.0);
		case 3:
			return ( 1.0 - ( (n * (n * n * 15739 + 789311) + 1376312657) & 0x7fffffff) / 1073741833.0);
		case 4:
			return ( 1.0 - ( (n * (n * n * 15749 + 789323) + 1376312687) & 0x7fffffff) / 1073741839.0);
		case 5:
			return ( 1.0 - ( (n * (n * n * 15761 + 789331) + 1376312689) & 0x7fffffff) / 1073741843.0);
		case 6:
			return ( 1.0 - ( (n * (n * n * 15767 + 789343) + 1376312753) & 0x7fffffff) / 1073741857.0);
		default:
			return ( 1.0 - ( (n * (n * n * 15773 + 789367) + 1376312783) & 0x7fffffff) / 1073741891.0);
	}
}

float cPerlinNoise::SmoothNoise(float fX, float fY, int iFunction)
{
	float fCorners = ( Noise(fX - 1, fY - 1, iFunction) + Noise(fX + 1, fY - 1, iFunction) + Noise(fX - 1, fY + 1, iFunction) + Noise(fX + 1, fY + 1, iFunction) ) / 16;
	float fSides   = ( Noise(fX - 1, fY    , iFunction) + Noise(fX + 1, fY    , iFunction) + Noise(fX    , fY - 1, iFunction) + Noise(fX    , fY - 1, iFunction) ) / 8;
	float fCenter  = Noise(fX, fY, iFunction) / 4;

	return fCorners + fSides + fCenter;
}

float cPerlinNoise::InterpolateNoise(float fX, float fY, int iFunction)
{
    int iX             = static_cast<int>(fX);
    float fFractionalX = fX - iX;

    int iY             = static_cast<int>(fY);
    float fFractionalY = fY - iY;

    float v1 = SmoothNoise(iX    , iY    , iFunction);
    float v2 = SmoothNoise(iX + 1, iY    , iFunction);
    float v3 = SmoothNoise(iX    , iY + 1, iFunction);
    float v4 = SmoothNoise(iX + 1, iY + 1, iFunction);

    float i1 = Interpolation(v1, v2, fFractionalX);
    float i2 = Interpolation(v3, v4, fFractionalX);

    return Interpolation(i1, i2, fFractionalY);
}

float cPerlinNoise::Interpolation(float fA, float fB, float fX)
{
    float ft = fX * 3.1415927;
    float f  = (1 - std::cos(ft)) * 0.5;

    return fA * (1 - f) + fB * f;
}

float cPerlinNoise::CreateNoise(float fX)
{
    float total = 0.0;

    for(int i = 0; i < m_iOctaves - 1; ++i)
    {
        float frequency = std::pow(2.0, i);
        float amplitude = std::pow(static_cast<float>(m_iPersistence), i);

        total += InterpolateNoise(fX * frequency, fX * frequency, i) * amplitude;
    }

    return total;
}