#pragma once

class cPerlinNoise
{
public:
	cPerlinNoise(int iOctaves, float fPersistence);
	float CreateNoise(float fX);

private:
	int m_iOctaves;
	float m_fPersistence;

	float SmoothNoise(float fX, float fY, int iFunction);
	float Noise(int iX, int iY, int iFunction);
	float InterpolateNoise(float fX, float fY, int iFunction);
	float Interpolation(float fA, float fB, float fX);
};