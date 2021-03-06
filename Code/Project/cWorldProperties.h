struct cWorldProperties
{
	static float GetPowerUpTimerMax()
	{
		return 3.0f;
	}

	static float GetPowerUpTimerStart()
	{
		return 0.0f;
	}

	static int GetTimeBarLengthInPixels()
	{
		return 150;
	}

	static float GetWorldMoveSpeed()
	{
		return 1.9f;
	}

	static float GetTerrainHeightFrequency()
	{
		return 0.01f;
	}

	static float GetJumpTimePenalty()
	{
		return 0.1f;
	}

	static float GetAlarmTriggerTime()
	{
		return 2.0f;
	}

	static float GetPowerUpTime()
	{
		return 11.5f;
	}

	static float GetBackgroundShapeWidth()
	{
		return 75.f;
	}

	static float GetHighScoreMultiplyOffset ()
	{
		return 0.15f;
	}
	static float GetHighScoreMultiplierTimerMax()
	{
		return 15.0f;
	}

	static std::string GetGameName ( )
	{
		return "Stahlholz";
	}

	static float GetInfoDisplayTimeMax()
	{
		return 5.f;
	}
};