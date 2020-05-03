#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Explosion
{
public:
	explicit Explosion(unsigned short int const posX, unsigned short int const posY); // Explicit constructor : we can't create an implicit object in function parameters
	~Explosion() = default; // We exprime the default destructor

	Explosion(Explosion const& other) = delete; // We delete the copy constructor
	void operator=(Explosion const& other) = delete; // We delete the = operator to avoid object copies

	void display(sf::RenderWindow& window);
	bool isFinished() const;

protected:
	unsigned short int x{};
	unsigned short int y{};
	bool finished{ false };
	sf::Sprite sprite{};
	sf::Sound sound{};
	sf::Clock age{};

	static constexpr std::string_view PATH_TEXTURE{ "assets/explosion.png" };
	static constexpr std::string_view PATH_SOUND{ "assets/explosion.wav" };
	static constexpr float LIFE_SPAN{ 0.2f };
};

#endif // !EXPLOSION_HPP
