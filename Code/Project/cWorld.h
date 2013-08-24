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

	void Draw ( sf::RenderWindow* RW);

	void ChangeRemainingTime ( float deltaT);

private:
	cPlayer* m_pPlayer;

	float m_fTotalTime;
	float m_fRemainingTime;
	float m_fStartTime;

	float m_fWorldMoveSpeed;

	void MoveTiles( sf::Vector2f vecDelta);
	
	std::vector<cTile*> m_vecTiles;

	sf::Vector2u m_vecWorldSize;


};