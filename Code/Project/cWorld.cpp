#include<time.h>

#include "cUtility.h"
#include "cWorld.h"
#include "cPlayer.h"
#include "cRandom.h"

#include "cWorldProperties.h"

cWorld::cWorld()
{
	cWorld::m_PerlinNoise = new cPerlinNoise(2, 3.f);

	cWorld::m_bAlarmRunning = false;
	cWorld::m_bMultiplyEffectRunning = false;

	// Create background gradient
	CreateColorGradient();
	cWorld::m_BackgroundTexture.loadFromImage(cWorld::m_BackgroundGradient);
	cWorld::m_BackgroundSprite.setTexture(cWorld::m_BackgroundTexture);

	// load Sounds
	cWorld::m_BackgroundMusicIntro.openFromFile("sfx/LD27_Intro.ogg");
	cWorld::m_BackgroundMusicLoop.openFromFile("sfx/LD27_Loop.ogg");
	cWorld::m_BackgroundMusicLoop.setLoop(true);

	cWorld::m_SoundBufferMultiplier.loadFromFile("sfx/multiply.wav");
	cWorld::m_SoundMultiplier.setBuffer(cWorld::m_SoundBufferMultiplier);
	cWorld::m_SoundMultiplier.setVolume(15.f);


	/// timing Vars
	cWorld::ResetTimers();
	m_fInfoDisplayTimer = 0.f;

	
	/// Time Bar
	cWorld::m_fTotalTimeBarLength = cWorldProperties::GetTimeBarLengthInPixels();
	cWorld::m_pTimeBar = new sf::RectangleShape(sf::Vector2f(m_fTotalTimeBarLength, 30));
	cWorld::m_pTimeBar->setFillColor(sf::Color::Green);
	cWorld::m_pTimeBar->setPosition(10, 10);

	// Shape Alarm
	m_ShapeAlarm = new sf::RectangleShape(sf::Vector2f(cWorldProperties::GetTimeBarLengthInPixels(),30));
	m_ShapeAlarm->setFillColor(sf::Color::Transparent);
	m_ShapeAlarm->setOutlineThickness(3);
	m_ShapeAlarm->setOutlineColor(sf::Color::Transparent);
	m_ShapeAlarm->setPosition(10, 10);

	// Shape Mulitply
	m_ShapeMulitply = new sf::CircleShape(10.f, 50);
	m_ShapeMulitply->setFillColor(sf::Color::Transparent);
	m_ShapeMulitply->setOutlineThickness(3);
	m_ShapeMulitply->setOutlineColor(sf::Color::Transparent);
	m_ShapeMulitply->setPosition(585.f, 5.f);
	m_ShapeMulitply->setOrigin(10.f, 10.f);
	

	/// world Vars
	cWorld::m_fWorldMoveSpeed = cWorldProperties::GetWorldMoveSpeed();
	m_vecCumulativeWorldMovement = sf::Vector2f(0.f, 0.f);
	CreateWorld();
	m_bGameOver = false;
	m_bClose = false;
	m_bCredits = false;


	/// Player
	cWorld::m_pPlayer = new cPlayer();
	cWorld::m_pPlayer->SetWorld(this);
	ResetPlayer();


	/// Power Up
	cWorld::m_fPowerUpTimerMax = cWorldProperties::GetPowerUpTimerMax();
	cWorld::m_fPowerUpTimer = cWorldProperties::GetPowerUpTimerStart();
	m_bPowerUpSpawned = false;

	cWorld::m_BackgroundMusicIntro.play();

	font.loadFromFile("gfx/font.ttf");
}

cWorld::~cWorld()
{

	DeleteTiles();
	delete cWorld::m_pPlayer;
	delete cWorld::m_pTimeBar;
	delete cWorld::m_PerlinNoise;
	delete cWorld::m_ShapeAlarm;
	m_vecBackgroundShapes.clear();
}

void cWorld::DeleteTiles()
{
	std::vector<cTile*>::iterator it;
	for (	it = m_vecTiles.begin();
			it != m_vecTiles.end();
			++it)
	{
		delete (*it);
		it = m_vecTiles.erase(it);
		if(it ==m_vecTiles.end())
			break;
	}
	cWorld::m_vecTiles.clear();
	m_vecBackgroundShapes.clear();

}

void cWorld::CreateWorld ()
{
	// Create a randomly generated World
	DeleteTiles();
	
	// Generate the Bottom
	cTile* t_pTile = NULL;
	
	for (int i = 0; i < 50; ++i)
	{
		float xCoord = cTile::s_iTileSizeInPixels * i;
		float yCoord = 600 - cTile::s_iTileSizeInPixels * GetTerrainHeight(cWorldProperties::GetTerrainHeightFrequency() * 20 * i);

		t_pTile= new cTile(cTile::EARTH_TOP);
		t_pTile->SetPosition(
			sf::Vector2f(
				xCoord,
				yCoord
		));
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

	cPerlinNoise Perlin(3,3);
	// Create Background Shapes
	for (int i = 0; i <= 45; ++i)
	{
		
		float t_fHeight = (Perlin.CreateNoise(cWorldProperties::GetBackgroundShapeWidth()*i) + 1.f ) * 100.f+ 350.f;
		sf::RectangleShape t_shapeBackground(sf::Vector2f(cWorldProperties::GetBackgroundShapeWidth(), t_fHeight ));
		t_shapeBackground.setPosition(cWorldProperties::GetBackgroundShapeWidth()*i, 600.f- t_fHeight);
		t_shapeBackground.setFillColor(sf::Color(140, 140, 140, 105));
		m_vecBackgroundShapes.push_back(t_shapeBackground);
	}
}

void cWorld::GetInput (sf::Event& Event)
{
	if (!m_bGameOver)
	{
		cWorld::m_pPlayer->GetInput(Event);
		if (Event.type == sf::Event::KeyReleased)
		{
			if(Event.key.code == sf::Keyboard::Escape)
			{
				EndGame();
			}
		}
	}
	else
	{
		if (Event.type == sf::Event::KeyReleased)
		{
			if(Event.key.code == sf::Keyboard::Escape)
			{
				cWorld::m_bClose = true;
			}
			else if (Event.key.code == sf::Keyboard::C)
			{
				m_bCredits = !m_bCredits;
			}
			else if (Event.key.code != sf::Keyboard::Space)
			{
				cWorld::RestartGame();
			}
		}
	}
}

void cWorld::Update (float deltaT)
{
	if (!m_bGameOver)
	{
		if (m_BackgroundMusicIntro.getStatus() == sf::Sound::Stopped && m_BackgroundMusicLoop.getStatus() == sf::Sound::Stopped )
		{
			m_BackgroundMusicLoop.play();
		}

		cWorld::m_pPlayer->Update(deltaT);
		cWorld::m_fTotalTime += deltaT * cWorld::m_fHighscoreMultiplier;
		cWorld::m_fRemainingTime -= deltaT;
		cWorld::m_fHighscoreMultiplierTimer += deltaT;
		cWorld::m_fInfoDisplayTimer += deltaT;

		if ( m_fRemainingTimeLastFrame >= cWorldProperties::GetAlarmTriggerTime() && m_fRemainingTime < cWorldProperties::GetAlarmTriggerTime())
		{
			cWorld::TriggerLastManStandingEffect();
		}


		
		if (cWorld::m_fHighscoreMultiplierTimer >= cWorldProperties::GetHighScoreMultiplierTimerMax())
		{
			IncreaseHighScoreMultiplier();
		}


		if(m_fPowerUpTimer >= 0.f)
		{
			cWorld::m_fPowerUpTimer -= deltaT;
		}
		if (m_fPowerUpTimer <= 0 && !m_bPowerUpSpawned)	// powerup is spawned
		{
			cWorld::RepositionPowerUp();
		}
		if (m_bPowerUpSpawned && m_PowerUp.GetPosition().x < -20)
		{
			ResetPowerUpPosition();	// move it out of the way
		}


		if (m_bAlarmRunning)
		{
			//std::cout << cWorld::m_ShapeAlarm->getScale().x  << std::endl;
			cWorld::m_ShapeAlarm->scale(cWorld::m_ShapeAlarm->getScale().x + 0.002f, cWorld::m_ShapeAlarm->getScale().y + 0.002f);
		}
		if (m_bMultiplyEffectRunning)
		{
			cWorld::m_ShapeMulitply->scale(cWorld::m_ShapeMulitply->getScale().x + 0.0001f, cWorld::m_ShapeMulitply->getScale().y + 0.0001f);
		}


		cWorld::MoveTheWorld(deltaT);
		
		cWorld::UpdateTimeBar();
		
		m_fRemainingTimeLastFrame = cWorld::m_fRemainingTime;
	}
}

void cWorld::IncreaseHighScoreMultiplier()
{
	cWorld::m_fHighscoreMultiplier += cWorldProperties::GetHighScoreMultiplyOffset();
	cWorld::m_fHighscoreMultiplierTimer = 0.0f;
	cWorld::m_SoundMultiplier.play();
	m_fMusicalPitch +=  cWorldProperties::GetHighScoreMultiplyOffset() * 0.1f;
	cWorld::m_BackgroundMusicLoop.setPitch(m_fMusicalPitch);
	cWorld::TriggerMultiplyEffect();
}

void cWorld::CreateColorGradient()
{
	cWorld::m_Gradient.insert(0.0, sf::Color(140, 217, 168));
	cWorld::m_Gradient.insert(1.0, sf::Color(221, 247, 130));

	sf::Color* tab = new sf::Color[600];
	cWorld::m_Gradient.fillTab(tab, 600);

	cWorld::m_BackgroundGradient.create(800, 600);
	
	for(int i = 0; i < 800; i++)
	{
		for(int j = 0; j < 600; j++)
		{
			cWorld::m_BackgroundGradient.setPixel(i, j, tab[j]);
		}		
	}
}


void cWorld::MoveTheWorld(float deltaT)
{
	sf::Vector2f t_vecTileMovement = sf::Vector2f(-1.f,0.f) * cWorld::m_fWorldMoveSpeed * cWorld::m_fHighscoreMultiplier;
	cWorld::MoveTiles(t_vecTileMovement);
	cWorld::MoveBackground(t_vecTileMovement);
}

void cWorld::UpdateTimeBar()
{
	float t_fTimeBarLenghtFactor = m_fRemainingTime/ 10.f;
	if (t_fTimeBarLenghtFactor < 0.f)
	{
		t_fTimeBarLenghtFactor = 0.f;
		EndGame();
	}
	else if ( t_fTimeBarLenghtFactor > 1.f )
	{
		t_fTimeBarLenghtFactor = 1.f;

		// additional: draw time more than 10 seconds next to time bar
	}
	m_pTimeBar->setScale(t_fTimeBarLenghtFactor, 1.f);
}


void cWorld::Draw ( sf::RenderWindow* RW)
{
	if (!m_bGameOver)
	{
		RW->draw(cWorld::m_BackgroundSprite);

		std::vector<sf::RectangleShape>::iterator itShapes;
		for (	itShapes = m_vecBackgroundShapes.begin();
				itShapes != m_vecBackgroundShapes.end();
				++itShapes)
		{
			RW->draw((*itShapes));
		}

		// Render tiles
		std::vector<cTile*>::iterator it;
		for (	it = m_vecTiles.begin();
				it != m_vecTiles.end();
				++it)
		{
			(*it)->Draw(RW);
		}

		// Render player and powerup
		cWorld::m_pPlayer->Draw(RW);
		cWorld::m_PowerUp.Draw(RW);




		// Render time bar
		cWorld::DrawTime(RW);
		
		RW->draw(*m_ShapeAlarm);
		RW->draw(*m_ShapeMulitply);

		// create and Draw the fonts

		/// Total Time
		sf::Text t_sftextTotalTime("", font);
		t_sftextTotalTime.setString("Total " + tostr(cWorld::m_fTotalTime));
		t_sftextTotalTime.setPosition(sf::Vector2f(640.f, 5.f));
		RW->draw(t_sftextTotalTime);

		/// Time Multiplier
		t_sftextTotalTime.setString("x" + tostr(cWorld::m_fHighscoreMultiplier));
		t_sftextTotalTime.setPosition(sf::Vector2f(585.f, 5.f));
		RW->draw(t_sftextTotalTime);

		if (m_fInfoDisplayTimer <= cWorldProperties::GetInfoDisplayTimeMax())
		{
			t_sftextTotalTime.setString("[Space] to Jump (costs time)");
			t_sftextTotalTime.setScale(1.25f, 1.25f);
			t_sftextTotalTime.setPosition(400.f - t_sftextTotalTime.getGlobalBounds().width/2 , 230.f - t_sftextTotalTime.getGlobalBounds().height/2);
			RW->draw(t_sftextTotalTime);

			t_sftextTotalTime.setString("[ESC] to Quit");
			t_sftextTotalTime.setScale(1.25f, 1.25f);
			t_sftextTotalTime.setPosition(400.f - t_sftextTotalTime.getGlobalBounds().width/2 , 280.f - t_sftextTotalTime.getGlobalBounds().height/2);
			RW->draw(t_sftextTotalTime);


			t_sftextTotalTime.setString("Get all The Power Ups!");
			t_sftextTotalTime.setScale(1.25f, 1.25f);
			t_sftextTotalTime.setPosition(400.f - t_sftextTotalTime.getGlobalBounds().width/2 , 310.f - t_sftextTotalTime.getGlobalBounds().height/2);
			RW->draw(t_sftextTotalTime);
		}


		/// Additional Time
		if (cWorld::m_fRemainingTime > 10.f)
		{
			sf::Text t_sftextAdditionalTime("", font);
			t_sftextAdditionalTime.setString("+" + tostr(cWorld::m_fRemainingTime - 10.f));
			t_sftextAdditionalTime.setPosition(sf::Vector2f(165.f, 5.f));
			RW->draw(t_sftextAdditionalTime);
		}
	}
	else	// Game Over
	{
		if (!m_bCredits)
		{
			RW->clear();

			sf::Text t_sftextGoodJob("Game Over", font);
			t_sftextGoodJob.setScale(1.5f, 1.5f);
			t_sftextGoodJob.setPosition(sf::Vector2f(400.f- t_sftextGoodJob.getGlobalBounds().width/2, 100.f));
			RW->draw(t_sftextGoodJob);

			t_sftextGoodJob.setString("You scored");
			t_sftextGoodJob.setScale(0.75f, 0.75f);
			t_sftextGoodJob.setPosition(sf::Vector2f(400.f- t_sftextGoodJob.getGlobalBounds().width/2, 250.f));
			RW->draw(t_sftextGoodJob);


			t_sftextGoodJob.setString(tostr(cWorld::m_fTotalTime));
			t_sftextGoodJob.setScale(1.5f, 1.5f);
			t_sftextGoodJob.setPosition(sf::Vector2f(400.f- t_sftextGoodJob.getGlobalBounds().width/2, 290.f));
			RW->draw(t_sftextGoodJob);
		
			t_sftextGoodJob.setString("Seconds");
			t_sftextGoodJob.setScale(0.75f, 0.75f);
			t_sftextGoodJob.setPosition(sf::Vector2f(400.f- t_sftextGoodJob.getGlobalBounds().width/2, 340.f));
			RW->draw(t_sftextGoodJob);


			t_sftextGoodJob.setString("Replay => [Enter]");
			t_sftextGoodJob.setScale(1.5f, 1.5f);
			t_sftextGoodJob.setPosition(sf::Vector2f(400.f- t_sftextGoodJob.getGlobalBounds().width/2, 400.f));
			RW->draw(t_sftextGoodJob);


			t_sftextGoodJob.setString("[C]redits");
			t_sftextGoodJob.setScale(0.75f, 0.75f);
			t_sftextGoodJob.setPosition(10.f, 550.f);
			RW->draw(t_sftextGoodJob);
		}
		else
		{
			sf::Text t_sftextCredis(cWorldProperties::GetGameName(), font);
			t_sftextCredis.setScale(2.0f, 2.0f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 50.f));
			RW->draw(t_sftextCredis);



			t_sftextCredis.setString("created by");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 120.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Julian Dinges");
			t_sftextCredis.setScale(1.5f, 1.5f);
			t_sftextCredis.setPosition(sf::Vector2f(200.f- t_sftextCredis.getGlobalBounds().width/2, 150.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Programming");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(200.f- t_sftextCredis.getGlobalBounds().width/2, 200.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Graphics");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(200.f- t_sftextCredis.getGlobalBounds().width/2, 220.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Pizza & Video");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(200.f- t_sftextCredis.getGlobalBounds().width/2, 240.f));
			RW->draw(t_sftextCredis);




			t_sftextCredis.setString("Simon Weis");
			t_sftextCredis.setScale(1.5f, 1.5f);
			t_sftextCredis.setPosition(sf::Vector2f(600.f- t_sftextCredis.getGlobalBounds().width/2, 150.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Programming");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(600.f- t_sftextCredis.getGlobalBounds().width/2, 200.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Sound");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(600.f- t_sftextCredis.getGlobalBounds().width/2, 220.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Blackboard & Gummibears");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(600.f- t_sftextCredis.getGlobalBounds().width/2, 240.f));
			RW->draw(t_sftextCredis);



			t_sftextCredis.setString("using");
			t_sftextCredis.setScale(1.5f, 1.5f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 290.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Visual Studio 2010 \t Cubase 5 & sfxr \t aseprite");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 340.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("SFML 2.1");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 360.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Thanks to");
			t_sftextCredis.setScale(1.5f, 1.5f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 400.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("Families & Friens for their great Support");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 450.f));
			RW->draw(t_sftextCredis);

			t_sftextCredis.setString("LD for this great Opportunity. Keep it up!");
			t_sftextCredis.setScale(0.75f, 0.75f);
			t_sftextCredis.setPosition(sf::Vector2f(400.f- t_sftextCredis.getGlobalBounds().width/2, 470.f));
			RW->draw(t_sftextCredis);

		}
	}
}

void cWorld::ChangeRemainingTime ( float deltaT)
{
	m_fRemainingTime += deltaT;
}

void cWorld::MoveBackground(sf::Vector2f Delta)
{
	std::vector<sf::RectangleShape>::iterator itShapes;
	for (	itShapes = m_vecBackgroundShapes.begin();
			itShapes != m_vecBackgroundShapes.end();
			++itShapes)
	{
		(itShapes)->setPosition((itShapes)->getPosition() + Delta * 0.5f);
		if( itShapes->getPosition().x <= - itShapes->getSize().x)
		{
			itShapes->setPosition(itShapes->getPosition().x + itShapes->getSize().x * m_vecBackgroundShapes.size(), itShapes->getPosition().y);
		}
	}
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

		if(deltaX > -50 && deltaX < 50)
		{
			if(deltaY > -50 && deltaY < 50)
			{
				vecReturn.push_back((*it));
			}
		}
	}

	return vecReturn;
}

int cWorld::GetTerrainHeight(float xValue)
{
	float noise = cWorld::m_PerlinNoise->CreateNoise(xValue);
	return static_cast<int>((noise + 1.f) / 2.f * 26.f) + 1;
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

void cWorld::EndGame()
{
	m_bGameOver = true;
	m_BackgroundMusicIntro.stop();
	m_BackgroundMusicLoop.stop();
}

void cWorld::RestartGame()
{
	CreateWorld();
	
	m_bGameOver = false;
	ResetTimers();
	ResetPlayer();
}

void cWorld::ResetTimers()
{
	cWorld::m_fStartTime = 10.f;
	cWorld::m_fTotalTime = 0.f;
	cWorld::m_fRemainingTime = m_fStartTime;
	cWorld::m_fRemainingTimeLastFrame = m_fRemainingTime;
	m_fHighscoreMultiplierTimer = 0.f;
}

void cWorld::ResetPlayer()
{
	cWorld::m_pPlayer->SetPosition(sf::Vector2f(300.f, 30.f));
	cWorld::m_fHighscoreMultiplier = 1.0f;
	cWorld::m_fMusicalPitch = 1.0f;
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
	float t_fPosYMax = 600.f - (cTile::s_iTileSizeInPixels* cWorld::GetTerrainHeight(cWorldProperties::GetTerrainHeightFrequency() * t_fPosX) + 50.f);
	float t_fPosY = cRandom::GetRandomFloat(10, t_fPosYMax);
	cWorld::m_PowerUp.SetPosition(sf::Vector2f( t_fPosX , t_fPosY));
	m_bPowerUpSpawned = true;
	
}

void cWorld::TriggerLastManStandingEffect()
{
	m_ShapeAlarm->setOutlineColor(sf::Color::Red);
	m_ShapeAlarm->setScale(1.0f, 1.0f);
	cWorld::m_bAlarmRunning = true;
}

void cWorld::TriggerMultiplyEffect()
{
	m_ShapeMulitply->setOutlineColor(sf::Color(100,250, 100));
	m_ShapeMulitply->setScale(1.0f, 1.0f);
	cWorld::m_bMultiplyEffectRunning = true;
}