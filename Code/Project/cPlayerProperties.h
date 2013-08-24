#include <SFML/Graphics.hpp>

struct cPlayerProperties
{
	static float GetJumpVelocity()
	{
		return -300.0f;
	}

	static float GetFrictionCoefficient()
	{
		return 1.f;
	}

	static float GetFallingVelocity()
	{
		return 2.0f;
	}
};