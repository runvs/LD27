#include "cWorld.h"

#include "cPlayer.h"

cWorld::cWorld()
{
	m_fStartTime = 10.f;
	m_fTotalTime = 0.f;
	m_fRemainingTime = m_fStartTime;

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
	m_fTotalTime += deltaT;
	m_fRemainingTime -= deltaT;
}

void cWorld::Draw ( sf::RenderWindow* RW)
{
	cWorld::m_pPlayer->Draw(RW);
}

void cWorld::ChangeRemainingTime ( float deltaT)
{
	m_fRemainingTime += deltaT;
}