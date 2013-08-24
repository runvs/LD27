#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include "cTile.h"



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

private:
	cPlayer* m_pPlayer;

	float m_fTotalTime;
	float m_fRemainingTime;
	float m_fStartTime;

	float m_fWorldMoveSpeed;

	void MoveTiles( sf::Vector2f vecDelta);
	
	std::vector<cTile*> m_vecTiles;

	sf::Vector2f m_vecIncrementalWorldMovement;
	sf::Vector2f m_vecKumulativeWorldMovement;

	void DrawTime (sf::RenderWindow* RW);

	sf::Shape* m_pTimeBar;
	float m_fTotalTimeBarLength;

	void EndGame(float fScore);

};