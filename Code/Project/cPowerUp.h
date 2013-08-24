#pragma once
#include <SFML/Graphics.hpp>


class cPowerUp
{
public:
	cPowerUp();
	void Draw (sf::RenderWindow* renderWindow);

	// Setter and getter for tile position
	void SetPosition(sf::Vector2f vecPos);
	sf::Vector2f GetPosition();

	// Getter for the tile's sprite address
	inline sf::Sprite& GetSprite() { return m_Sprite; };

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;

};