#include "../include/Explosion.hpp"
#include "../include/ResourcesManager.hpp"

Explosion::Explosion(unsigned short int const posX, unsigned short int const posY) : x{ posX }, y{ posY }
{
	sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_TEXTURE));
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

	sound.setBuffer(ResourcesManager<sf::SoundBuffer>::getResource(PATH_SOUND));
	sound.play();
}

void Explosion::display(sf::RenderWindow& window)
{
	if (age.getElapsedTime().asSeconds() < LIFE_SPAN)
	{
		sprite.setScale(age.getElapsedTime().asSeconds() / LIFE_SPAN, age.getElapsedTime().asSeconds() / LIFE_SPAN);
		window.draw(sprite);
	}
	else
	{
		sprite.setScale(0, 0);
		if (sound.getStatus() == sf::Sound::Stopped)
		{
			finished = true;
		}
	}
}

bool Explosion::isFinished() const
{
	return finished;
}
