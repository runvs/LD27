class cPerlinNoise
{
public:
	cPerlinNoise(int iOctaves, int iPersistence);
	float CreateNoise(float fX);

private:
	int m_iOctaves;
	int m_iPersistence;

	float SmoothNoise(float fX, float fY, int iFunction);
	float Noise(int iX, int iY, int iFunction);
	float InterpolateNoise(float fX, float fY, int iFunction);
	float Interpolation(float fA, float fB, float fX);
};