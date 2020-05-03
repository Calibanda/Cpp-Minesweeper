#ifndef INFOBAR_HPP
#define INFOBAR_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include "Size.hpp"

class Game;

class InfoBar
{
public:
	explicit InfoBar(Game* ptrGame, Size sizeWindow, unsigned short int const height, unsigned short int const mines);
	~InfoBar() = default;

	InfoBar(InfoBar const& other) = delete; //We delete the copy constructor
	void operator=(InfoBar const& other) = delete; //We delete the = operator to avoid object copies

	void updateSize(Size const& newSize);
	void incrementNumberFlags(short int const incrementNumber);
	void startChrono();
	unsigned int stopChrono();
	void update(sf::RenderWindow& window);
	void display(sf::RenderWindow& window);

private:
	Game* game;
	Size sizeWindow;
	unsigned short int height;
	unsigned short int nbMines;

	unsigned short int nbFlags{ 0 };
	bool chronoIsStarted{ false };
	bool chronoIsStoped{ false };
	unsigned int time{};
	sf::Clock chrono{};
	sf::Text minesCounter{};
	sf::Text textChronometer{};
	sf::Font font{};

	static constexpr std::string_view PATH_FONT{ "assets/IMPACTED.ttf" };
	static constexpr int MARGIN_X{ 25 };
	static constexpr int MARGIN_Y{ 50 };
	const sf::Color COLOR_TEXT{ 185, 185, 185 };
};

#endif // !INFOBAR_HPP
