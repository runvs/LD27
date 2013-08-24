#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include "cTile.h"
#include "cPowerUp.h"


class cPlayer;


class cWorld
{
public:

	cWorld();
	~cWorld();

	void LoadWorld ();

	void GetInput (sf::Event& Event);

	void Update (float deltaT);

	void Draw (sf::RenderWindow* RW);

	void ChangeRemainingTime (float deltaT);

	std::vector<cTile*> GetTilesInProximity(sf::Vector2f position);

	inline sf::Sprite& GetPowerUp(void) {return m_PowerUp.GetSprite();};

	void ResetPowerUpPosition();

	inline bool GetClose() {return m_bClose;};

private:
	cPlayer* m_pPlayer;

	cPowerUp m_PowerUp;
	bool m_bPowerUpSpawned;

	float m_fTotalTime;
	float m_fRemainingTime;
	float m_fStartTime;
	
	float m_fPowerUpTimer;
	float m_fPowerUpTimerMax;

	float m_fWorldMoveSpeed;

	void MoveTiles( sf::Vector2f vecDelta);
	
	std::vector<cTile*> m_vecTiles;

	sf::Vector2f m_vecIncrementalWorldMovement;
	sf::Vector2f m_vecCumulativeWorldMovement;

	int GetTerrainHeight(float xValue);
	void AddTilesToTheEnd();

	void DrawTime (sf::RenderWindow* RW);

	sf::Shape* m_pTimeBar;
	float m_fTotalTimeBarLength;

	bool m_bGameOver;
	bool m_bClose;
	void EndGame(float fScore);
	void RestartGame();
	void ResetTimers();
	void ResetPlayer();


	void RepositionPowerUp();


};