


#include <SFML/Graphics.hpp>

#include "cWorld.h"
#include "cPlayer.h"

int main()
{
	sf::Clock MyClock;
	cWorld World;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "LD27!");
	window->setFramerateLimit(100);
	window->setVerticalSyncEnabled(true);

    while (window->isOpen())
    {
		float  t_fElapsedTimeInSeconds = MyClock.getElapsedTime().asSeconds();
		MyClock.restart();
        sf::Event MyEvent;
        while (window->pollEvent(MyEvent))
        {
            if (MyEvent.type == sf::Event::Closed)
                window->close();

			World.GetInput(MyEvent);
        }

		World.Update(t_fElapsedTimeInSeconds);

		window->clear();

		World.Draw(window);
		window->display();
		if (World.GetClose())
		{
			window->close();
			break;
		}

    }
	delete window;

    return 0;
}