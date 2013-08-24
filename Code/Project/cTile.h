#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class cTile
{
public:
	enum eTileType
	{
		GRASS_1, GRASS_2, EARTH_TOP, EARTH_BELOW, BLOCK
	};

	cTile(cTile::eTileType tileType);

	static int s_iTileSizeInPixels;

	void Draw (sf::RenderWindow* renderWindow);

	// Setter and getter for tile position
	void SetPosition(sf::Vector2f vecPos);
	sf::Vector2f GetPosition();

	// Setter and getter for tile index
	void SetIndex(sf::Vector2i vector);
	sf::Vector2i GetIndex();

	// Getter for the tile's sprite address
	sf::Sprite& GetSprite() { return m_Sprite; };

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::Vector2i m_Index;
	sf::Vector2f m_Position;

	std::string GetFileNameByTileType(cTile::eTileType tileType);
};