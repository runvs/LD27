#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class cWorld;

class cPlayer
{
public:
	cPlayer();

	void LoadPlayer();

	void GetInput (sf::Event& Event);

	void Update (float deltaT);

	void Draw ( sf::RenderWindow* RW);

	void Move (sf::Vector2f vecDelta);

	inline void SetWorld ( cWorld* pWorld) {m_pWorld = pWorld;};

	inline void SetPosition ( sf::Vector2f vec) {m_vecPos = vec; m_Sprite.setPosition(vec);};

	bool DoesCollide ( void );

private:
	sf::Vector2f m_vecPos;
	sf::Vector2f m_vecVelocity;

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	void Jump ( void );
	void Attack ( void );
	sf::Vector2f GetOverlap(sf::Vector2f other);

	cWorld* m_pWorld;
	
	sf::Vector2f m_vecJumpVelocityAdd;


	void LoadSounds ();

	sf::SoundBuffer m_SoundBufferJump;
	sf::Sound m_SoundJump;

	sf::SoundBuffer m_SoundBufferPickUp;
	sf::Sound m_SoundPickUp;
};