#include "cUtility.h"
#include "cWorld.h"
#include "cPlayer.h"

cWorld::cWorld()
{
	/// timing Vars
	m_fStartTime = 10.f;
	m_fTotalTime = 0.f;
	m_fRemainingTime = m_fStartTime;

	m_fTotalTimeBarLength = 150;

	m_pTimeBar = new sf::RectangleShape(sf::Vector2f(m_fTotalTimeBarLength,30));

	m_pTimeBar->setFillColor(sf::Color::Green);
	m_pTimeBar->setPosition(10, 10);


	/// world Vars
	m_fWorldMoveSpeed = 1.5f;
	m_vecKumulativeWorldMovement = sf::Vector2f(0.f, 0.f);
	LoadWorld();

	/// Player
	cWorld::m_pPlayer = new cPlayer();
	cWorld::m_pPlayer->SetWorld(this);

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
			pTile= new cTile(cTile::EARTH_BELOW);
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

		// additional: draw time next to time bar
	}
	m_pTimeBar->setScale(t_fTimeBarLenghtFactor, 1.f);

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

	cWorld::DrawTime(RW);
}

void cWorld::ChangeRemainingTime ( float deltaT)
{
	m_fRemainingTime += deltaT;
}


void cWorld::MoveTiles( sf::Vector2f Delta)
{
	m_vecKumulativeWorldMovement -= Delta;
	m_vecIncrementalWorldMovement -= Delta;

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
		m_vecIncrementalWorldMovement.x -= cTile::s_iTileSizeInPixels;
		cTile* t_pTile = new cTile(cTile::EARTH_BELOW);

		t_pTile->SetPosition(sf::Vector2f(m_vecIncrementalWorldMovement.x + 49.f * cTile::s_iTileSizeInPixels, 600.f - cTile::s_iTileSizeInPixels ));

		m_vecTiles.push_back(t_pTile);
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

	//std::cout << vecReturn.size() << std::endl;
	return vecReturn;
}


void cWorld::DrawTime (sf::RenderWindow* RW)
{
	RW->draw(*m_pTimeBar);
}

void cWorld::EndGame(float fScore)
{

}