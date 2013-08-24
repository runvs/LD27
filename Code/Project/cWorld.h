#pragma once

#include <SFML/Graphics.hpp>
#include "cTile.h"

class cPlayer;


class cWorld
{
public:

	void LoadWorld ();

	void GetInput (sf::Event& Event);

	void Update (float deltaT);

	void Draw ( sf::RenderWindow* RW);

	cPlayer* m_pPlayer;
};