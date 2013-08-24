#include <SFML/Graphics.hpp>

struct cPlayerProperties
{
	static float GetJumpVelocity()
	{
		return -300.0f;
	}

	static float GetFrictionCoefficient()
	{
		return 0.95f;
	}

	static float GetFallingVelocity()
	{
		return 4.0f;
	}
};