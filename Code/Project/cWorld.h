#pragma once

#include <vector>
#include <SFML/Audio.hpp>

#include <SFML/Graphics.hpp>
#include "cTile.h"
#include "cPowerUp.h"
#include "cPerlinNoise.h"
#include "cColorScale.h"


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
	
	void EndGame();

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

	void MoveTheWorld(float deltaT);
	cPerlinNoise* m_PerlinNoise;
	cColorScale m_Gradient;

	void MoveTiles( sf::Vector2f vecDelta);
	void MoveBackground(sf::Vector2f Delta);
	
	void CreateColorGradient();
	sf::Image m_BackgroundGradient;
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	
	std::vector<cTile*> m_vecTiles;

	sf::Vector2f m_vecIncrementalWorldMovement;
	sf::Vector2f m_vecCumulativeWorldMovement;

	int GetTerrainHeight(float xValue);
	void AddTilesToTheEnd();

	void DrawTime (sf::RenderWindow* RW);

	std::vector<sf::RectangleShape> m_vecBackgroundShapes;

	
	void UpdateTimeBar();
	sf::Shape* m_pTimeBar;
	float m_fTotalTimeBarLength;

	bool m_bGameOver;
	bool m_bClose;
	void RestartGame();
	void ResetTimers();
	void ResetPlayer();

	void RepositionPowerUp();

	void DeleteTiles();


	sf::Music m_BackgroundMusicIntro;
	sf::Music m_BackgroundMusicLoop;

	sf::Font font;


};