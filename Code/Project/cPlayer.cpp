#include <string>

#include "cPlayer.h"
#include "cWorld.h"


cPlayer::cPlayer()
{
	LoadPlayer();
	m_vecPos = sf::Vector2f(300.f, 400.f);
	m_pWorld = NULL;
}

void cPlayer::LoadPlayer()
{
	cPlayer::m_Texture.loadFromFile("gfx/Player.png");
	cPlayer::m_Sprite.setTexture(cPlayer::m_Texture);
}


void cPlayer::GetInput (sf::Event& Event)
{
	if (Event.type == sf::Event::KeyReleased)
	{
		if (Event.key.code == sf::Keyboard::Space)
		{
			cPlayer::Jump();
		}
		if (Event.key.code == sf::Keyboard::LControl)
		{
			cPlayer::Attack();
		}
	}
}

void cPlayer::Update (float deltaT)
{
	sf::Vector2f t_vecPositionChange = cPlayer::m_vecVelocity * deltaT;
	cPlayer::Move(t_vecPositionChange);
}

void cPlayer::Draw ( sf::RenderWindow* RW)
{
	RW->draw(cPlayer::m_Sprite);
}

void cPlayer::Move (sf::Vector2f vecDelta)
{
	cPlayer::m_vecPos += vecDelta;
	cPlayer::m_Sprite.setPosition(m_vecPos);
}

void cPlayer::Jump ( void )
{

}

void cPlayer::Attack ( void )
{

}