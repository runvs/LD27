#include <string>
#include <iostream>

#include "cPlayer.h"
#include "cWorld.h"
#include "cPlayerProperties.h"
#include "cWorldProperties.h"
#include "cCollision.h"

cPlayer::cPlayer()
{
	LoadPlayer();
	LoadSounds();
	m_vecPos = sf::Vector2f(300.f, 400.f);
	m_vecJumpVelocityAdd = sf::Vector2f(0, cPlayerProperties::GetJumpVelocity());
	m_pWorld = NULL;
}

void cPlayer::LoadPlayer()
{
	cPlayer::m_Texture.loadFromFile("gfx/Player.png");
	cPlayer::m_Sprite.setTexture(cPlayer::m_Texture);
}

void cPlayer::LoadSounds()
{
	m_SoundBufferJump.loadFromFile("sfx/jump.wav");
	m_SoundJump.setBuffer(m_SoundBufferJump);
	m_SoundJump.setVolume(15.f);

	m_SoundBufferPickUp.loadFromFile("sfx/pickup.wav");
	m_SoundPickUp.setBuffer(m_SoundBufferPickUp);
	m_SoundPickUp.setVolume(25.f);
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


bool cPlayer::DoesCollide ( void )
{
	if(cPlayer::m_pWorld != NULL)
	{
		std::vector<cTile*> tilesInProximity = cPlayer::m_pWorld->GetTilesInProximity(cPlayer::m_vecPos);

		std::vector<cTile*>::iterator it;
		for(it = tilesInProximity.begin();
			it != tilesInProximity.end();
			++it)
		{
			// is there collission?
			if(Collision::BoundingBoxTest((*it)->GetSprite(), cPlayer::m_Sprite))
			{
				return true;
			}
		}
		return false;
	}
}

void cPlayer::Update (float deltaT)
{
	sf::Vector2f t_vecPositionChange = cPlayer::m_vecVelocity * deltaT;

	cPlayer::Move(t_vecPositionChange);

	cPlayer::m_vecVelocity.y += cPlayerProperties::GetFallingVelocity();
	cPlayer::m_vecVelocity *= cPlayerProperties::GetFrictionCoefficient();


	if(cPlayer::m_vecPos.x < 300)
	{
		if(cPlayer::m_vecPos.x <= -20.f)
		{
			cPlayer::m_pWorld->EndGame();
		}
		else
		{
			float factor = 1.f + ((300.f - cPlayer::m_vecPos.x) / 100);
			cPlayer::m_vecVelocity.x += factor;
		}
	}
	else
	{
		cPlayer::m_vecVelocity.x = 0;
	}
	


	// Collision detection
	if(cPlayer::m_pWorld != NULL)
	{
		std::vector<cTile*> tilesInProximity = cPlayer::m_pWorld->GetTilesInProximity(cPlayer::m_vecPos);

		std::vector<cTile*>::iterator it;
		for(it = tilesInProximity.begin();
			it != tilesInProximity.end();
			++it)
		{
			// is there collission?
			if(Collision::BoundingBoxTest((*it)->GetSprite(), cPlayer::m_Sprite))
			{
				// get the Overlap
				sf::Vector2f delta = (*it)->GetPosition() - cPlayer::m_vecPos;

				if( delta.y < 0.001f)
				{
					t_vecPositionChange.x = cTile::s_iTileSizeInPixels + delta.x;
				}

				cPlayer::Move(-t_vecPositionChange/2.0f);
				cPlayer::m_vecVelocity.y *= 0.1f;
			}
		}

		// Power Up 
		if( Collision::BoundingBoxTest(cPlayer::m_pWorld->GetPowerUp(), cPlayer::m_Sprite))
		{
			m_pWorld->ChangeRemainingTime(cWorldProperties::GetPowerUpTime());
			m_pWorld->ResetPowerUpPosition();
			cPlayer::m_SoundPickUp.play();
		}
	}
	else
	{
		std::cout << "No World" << std::endl;
	}
}

sf::Vector2f cPlayer::GetOverlap(sf::Vector2f otherMin)
{
	sf::Vector2f overlap;
	sf::Vector2f min = cPlayer::m_vecPos;
	sf::Vector2f max = sf::Vector2f(
		cPlayer::m_vecPos.x + cTile::s_iTileSizeInPixels,
		cPlayer::m_vecPos.y + cTile::s_iTileSizeInPixels
	);

	sf::Vector2f otherMax = sf::Vector2f(
		otherMin.x + cTile::s_iTileSizeInPixels,
		otherMin.y + cTile::s_iTileSizeInPixels
	);

	overlap.x = std::max(min.x, otherMin.x) - std::min(max.x, otherMax.x); 
	overlap.y = std::max(min.y, otherMin.y) - std::min(max.y, otherMax.y);              
	                
                
	if(otherMin.x < min.x)
	{
		overlap.x = -overlap.x;
	}

	if(otherMin.y < min.y)
	{
		overlap.y = -overlap.y;
	}
	                
	return overlap;
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
	cPlayer::m_SoundJump.play();
	cPlayer::m_vecVelocity += m_vecJumpVelocityAdd;
	if (cPlayer::m_pWorld)
	{
		cPlayer::m_pWorld->ChangeRemainingTime(-cWorldProperties::GetJumpTimePenalty());
	}
}

void cPlayer::Attack ( void )
{

}