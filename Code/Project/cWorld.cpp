#include "cUtility.h"
#include "cWorld.h"
#include "cPlayer.h"
#include "cRandom.h"

#include "cWorldProperties.h"

cWorld::cWorld()
{
	/// timing Vars
	cWorld::m_fStartTime = 10.f;
	cWorld::m_fTotalTime = 0.f;
	cWorld::m_fRemainingTime = m_fStartTime;

	
	/// Time Bar
	cWorld::m_fTotalTimeBarLength = cWorldProperties::GetTimeBarLengthInPixels();

	cWorld::m_pTimeBar = new sf::RectangleShape(sf::Vector2f(m_fTotalTimeBarLength,30));

	cWorld::m_pTimeBar->setFillColor(sf::Color::Green);
	cWorld::m_pTimeBar->setPosition(10, 10);


	/// world Vars
	cWorld::m_fWorldMoveSpeed = cWorldProperties::GetWorldMoveSpeed();
	m_vecCumulativeWorldMovement = sf::Vector2f(0.f, 0.f);
	LoadWorld();

	/// Player
	cWorld::m_pPlayer = new cPlayer();
	cWorld::m_pPlayer->SetWorld(this);

	/// Power Up
	cWorld::m_fPowerUpTimerMax = cWorldProperties::GetPowerUpTimerMax();
	cWorld::m_fPowerUpTimer = cWorldProperties::GetPowerUpTimerStart();
	m_bPowerUpSpawned = false;

}

cWorld::~cWorld()
{
	delete cWorld::m_pPlayer;
}

void cWorld::LoadWorld ()
{
	// create a randomly generated World

	
	// Generate the Bottom
	cTile* pTile =  NULL;
	
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 1; j < 2; ++j)
		{
			pTile= new cTile(cTile::EARTH_TOP);
			pTile->SetPosition(sf::Vector2f(cTile::s_iTileSizeInPixels * i, 600 - cTile::s_iTileSizeInPixels * j));  
			cWorld::m_vecTiles.push_back(pTile);
		}
	}
}

void cWorld::GetInput (sf::Event& Event)
{
	cWorld::m_pPlayer->GetInput(Event);
}

void cWorld::Update (float deltaT)
{
	cWorld::m_pPlayer->Update(deltaT);
	cWorld::m_fTotalTime += deltaT;
	cWorld::m_fRemainingTime -= deltaT;

	if(m_fPowerUpTimer >= 0.f)
	{
		cWorld::m_fPowerUpTimer -= deltaT;
	}

	sf::Vector2f t_vecTileMovement = sf::Vector2f(-1.f,0.f) * cWorld::m_fWorldMoveSpeed;
	cWorld::MoveTiles(t_vecTileMovement);

	float t_fTimeBarLenghtFactor = m_fRemainingTime/ 10.f;
	if (t_fTimeBarLenghtFactor < 0.f)
	{
		t_fTimeBarLenghtFactor = 0.f;
		EndGame(m_fTotalTime);
	}
	else if ( t_fTimeBarLenghtFactor > 1.f )
	{
		t_fTimeBarLenghtFactor = 1.f;

		// additional: draw time more than 10 seconds next to time bar
	}
	m_pTimeBar->setScale(t_fTimeBarLenghtFactor, 1.f);


	if (m_fPowerUpTimer <= 0 && !m_bPowerUpSpawned)	// powerup is spawned
	{
		cWorld::RepositionPowerUp();
	}
	if (m_bPowerUpSpawned && m_PowerUp.GetPosition().x < -20)
	{
		ResetPowerUpPosition();	// move it out of the way
	}
}

void cWorld::Draw ( sf::RenderWindow* RW)
{
	std::vector<cTile*>::iterator it;
	for (	it = m_vecTiles.begin();
			it != m_vecTiles.end();
			++it)
	{
		(*it)->Draw(RW);
	}
	cWorld::m_pPlayer->Draw(RW);
	cWorld::m_PowerUp.Draw(RW);

	cWorld::DrawTime(RW);
}

void cWorld::ChangeRemainingTime ( float deltaT)
{
	m_fRemainingTime += deltaT;
}


void cWorld::MoveTiles(sf::Vector2f Delta)
{
	m_vecCumulativeWorldMovement -= Delta;
	m_vecIncrementalWorldMovement -= Delta;


	m_PowerUp.SetPosition(m_PowerUp.GetPosition() + Delta);

	std::vector<cTile*>::iterator it;
	for (	it = m_vecTiles.begin();
			it != m_vecTiles.end();
			++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Delta);
		if ((*it)->GetPosition().x <= - cTile::s_iTileSizeInPixels * 3 )
		{
			delete (*it);
			it = m_vecTiles.erase(it);
			
			if (it == m_vecTiles.end())
				break;
		}
	}

	if( m_vecIncrementalWorldMovement.x >= cTile::s_iTileSizeInPixels)
	{
		AddTilesToTheEnd();
	}
}



std::vector<cTile*> cWorld::GetTilesInProximity(sf::Vector2f position)
{
	std::vector<cTile*> vecReturn;

	std::vector<cTile*>::iterator it;
	for(it = m_vecTiles.begin();
		it != m_vecTiles.end();
		++it)
	{
		float deltaX = position.x - (*it)->GetPosition().x;
		float deltaY = position.y - (*it)->GetPosition().y;

		if(deltaX > -100 && deltaX < 100)
		{
			if(deltaY > -100 && deltaY < 100)
			{
				vecReturn.push_back((*it));
			}
		}
	}

	return vecReturn;
}

int cWorld::GetTerrainHeight(float xValue)
{
	// Maybe perlin noise?

	// Magic!
	// Nope: sin ranges from -1 to 1 so we have to add 1 to avoid negative values.
	// Now we've got values ranging from 0 to 2 -> divide by 2.
	// We've got a vertical resolution of 600 pixels, each tile is 19 pixels high,
	// leave about 5 tiles empty and divide the remainder by 19. Voilà!
	// Oh and don't forget to add 1 afterwards. This is really magic.
	return static_cast<int>((std::sin(xValue) + 1.f) / 2.f * 26.f) + 1;
}

void cWorld::AddTilesToTheEnd()
{
	m_vecIncrementalWorldMovement.x -= cTile::s_iTileSizeInPixels;
	cTile* t_pTile = new cTile(cTile::EARTH_TOP);
	float xCoord = 49.f * cTile::s_iTileSizeInPixels - m_vecIncrementalWorldMovement.x;
	float yCoord = 600 - cTile::s_iTileSizeInPixels * GetTerrainHeight(cWorldProperties::GetTerrainHeightFrequency() * cWorld::m_vecCumulativeWorldMovement.x);

	t_pTile->SetPosition(sf::Vector2f(xCoord, yCoord));

	cWorld::m_vecTiles.push_back(t_pTile);

	// Add tiles to the bottom of the top block
	for(int i = 600;
		i > yCoord;
		i -= cTile::s_iTileSizeInPixels)
	{
		t_pTile = new cTile(cTile::EARTH_BELOW);
		t_pTile->SetPosition(sf::Vector2f(xCoord, i));
		cWorld::m_vecTiles.push_back(t_pTile);
	}
}

void cWorld::DrawTime (sf::RenderWindow* RW)
{
	RW->draw(*m_pTimeBar);
}

void cWorld::EndGame(float fScore)
{

}

void cWorld::ResetPowerUpPosition()
{
	m_PowerUp.SetPosition(sf::Vector2f(-200.f, 0.f));
	m_fPowerUpTimer = m_fPowerUpTimerMax;
	m_bPowerUpSpawned = false;
}

void cWorld::RepositionPowerUp()
{
	float t_fPosX = 49.f * cTile::s_iTileSizeInPixels - m_vecIncrementalWorldMovement.x;
	float t_fPosY = cRandom::GetRandomFloat(100.f,500.f);
	std::cout << t_fPosX << "\t" << t_fPosY << std::endl;
	cWorld::m_PowerUp.SetPosition(sf::Vector2f( t_fPosX , t_fPosY));
	m_bPowerUpSpawned = true;
	
}