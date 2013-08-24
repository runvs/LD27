#include <string>
#include <iostream>

#include "cPlayer.h"
#include "cWorld.h"


cPlayer::cPlayer()
{
	LoadPlayer();
	m_vecPos = sf::Vector2f(300.f, 400.f);
	m_vecJumpVelocityAdd = sf::Vector2f(0, -500.f);
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
	cPlayer::m_vecVelocity *= 0.99f;
	//std::cout << t_vecPositionChange.x << "\t" << t_vecPositionChange.y << std::endl;
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
	cPlayer::m_vecVelocity += m_vecJumpVelocityAdd;
}

void cPlayer::Attack ( void )
{

}