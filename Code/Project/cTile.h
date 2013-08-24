#pragma once

#include <SFML/Graphics.hpp>

class cTile
{
public:
	cTile();

	void SetPosition(sf::Vector2f vecPos);

	static int s_iTileSizeInPixels;

	void Draw ( sf::RenderWindow* RW);

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
};