#include "cUtility.h"
#include "cWorld.h"
#include "cPlayer.h"

cWorld::cWorld()
{
	/// timing Vars
	m_fStartTime = 10.f;
	m_fTotalTime = 0.f;
	m_fRemainingTime = m_fStartTime;


	/// world Vars
	m_vecWorldSize = sf::Vector2u(50, 40);
	m_fWorldMoveSpeed = 1.f;
	LoadWorld();

	/// Player
	cWorld::m_pPlayer = new cPlayer();

	

}

cWorld::~cWorld()
{
	delete cWorld::m_pPlayer;
}

void cWorld::LoadWorld ()
{
	// create a randomly generated World

	


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

}

void cWorld::ChangeRemainingTime ( float deltaT)
{
	m_fRemainingTime += deltaT;
}

void cWorld::MoveTiles( sf::Vector2f Delta)
{
	std::vector<cTile*>::iterator it;
	for (	it = m_vecTiles.begin();
			it != m_vecTiles.end();
			++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Delta);
	}
}