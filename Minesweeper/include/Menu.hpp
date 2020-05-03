#ifndef MENU_HPP
#define MENU_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Size.hpp"

class Game;

enum class Difficulty { EASY, MEDIUM, HARD, NONE };

class Menu
{
public:
	explicit Menu(Game* ptrGame, Size size);
	~Menu() = default;

	Menu(Menu const& other) = delete; //We delete the copy constructor
	void operator=(Menu const& other) = delete; //We delete the = operator to avoid object copies

	void updateTextDisplay(Size const& newSize, std::vector<unsigned int> const& bestTimes);
	void mouseLeftPress(sf::Vector2i const& mousePosition);
	void mouseLeftRelease(sf::Vector2i const& mousePosition);
	void updateMouseInfo(sf::RenderWindow& window);
	void display(sf::RenderWindow& window);

private:
	Game* game;
	Size sizeWindow;
	Difficulty lastPressedButton{ Difficulty::NONE };
	sf::Text textEasy{};
	sf::Text textMedium{};
	sf::Text textHard{};
	sf::Font font{};

	const sf::Color COLOR_TEXT{ 185, 185, 185 };
	static constexpr float RELATIVE_POSITION_TEXT_EASY_X{ 0.5f };
	static constexpr float RELATIVE_POSITION_TEXT_EASY_Y{ 0.65f };
	static constexpr float RELATIVE_POSITION_TEXT_MEDIUM_X{ 0.5f };
	static constexpr float RELATIVE_POSITION_TEXT_MEDIUM_Y{ 0.75f };
	static constexpr float RELATIVE_POSITION_TEXT_HARD_X{ 0.5f };
	static constexpr float RELATIVE_POSITION_TEXT_HARD_Y{ 0.85f };
};

#endif // !MENU_HPP
