#include "cPowerUp.h"


cPowerUp::cPowerUp()
{
	cPowerUp::m_Texture.loadFromFile("gfx/Powerup.png");
	cPowerUp::m_Sprite.setTexture(cPowerUp::m_Texture);
}

void cPowerUp::Draw (sf::RenderWindow* RW)
{
	RW->draw(cPowerUp::m_Sprite);
}

// Setter and getter for tile position
void cPowerUp::SetPosition(sf::Vector2f vecPos)
{
	cPowerUp::m_Position = vecPos;
	cPowerUp::m_Sprite.setPosition(vecPos);
}


sf::Vector2f cPowerUp::GetPosition()
{
	return cPowerUp::m_Position;
}