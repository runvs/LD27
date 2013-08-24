#include "cTile.h"


cTile::cTile(cTile::eTileType tileType)
{
	cTile::m_Texture.loadFromFile(cTile::GetFileNameByTileType(tileType));
	cTile::m_Sprite.setTexture(cTile::m_Texture);
}

int cTile::s_iTileSizeInPixels = 19;

void cTile::Draw (sf::RenderWindow* renderWindow)
{
	renderWindow->draw(cTile::m_Sprite);
}

// Setter and getter for tile position
void cTile::SetPosition(sf::Vector2f position)
{
	cTile::m_Position = position;
	cTile::m_Sprite.setPosition( position );
}

sf::Vector2f cTile::GetPosition()
{
	return cTile::m_Position;
}


std::string cTile::GetFileNameByTileType(cTile::eTileType tileType)
{
	switch(tileType)
	{
	case GRASS_1:
		return "gfx/Grass_1.png";
	case GRASS_2:
		return "gfx/Grass_2.png";
	case EARTH_TOP:
		return "gfx/Earth-Top.png";
	case EARTH_BELOW:
		return "gfx/Earth-Below.png";
	case BLOCK:
	default:
		return "gfx/Block.png";
	}
}