#include <SFML/Graphics.hpp>

struct cPlayerProperties
{
	static float GetJumpVelocity()
	{
		return -500.0f;
	}

	static float GetFrictionCoefficient()
	{
		return 0.99f;
	}

	static float GetFallingVelocity()
	{
		return 1.0f;
	}
};