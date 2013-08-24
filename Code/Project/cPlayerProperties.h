struct cPlayerProperties
{
	static float GetJumpVelocity()
	{
		return -350.0f;
	}

	static float GetFrictionCoefficient()
	{
		return 0.98f;
	}

	static float GetFallingVelocity()
	{
		return 9.81f;
	}

	static float GetPlayerRunFactor()
	{
		return 0.8f;
	}
};