#ifndef CELL_HPP
#define CELL_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <string_view>
#include <array>
#include "ResourcesManager.hpp"
#include "InfoBar.hpp"
#include "Coordinates.hpp"

class Game; // We can't include "Game.hpp" (recursive definition) so we use a forward declaration and include "Game.hpp" in "Cell.cpp"
class Grid; // We can't include "Grid.hpp" (recursive definition) so we use a forward declaration and include "Grid.hpp" in "Cell.cpp"

class Cell
{
public:
	explicit Cell(Game* attGame, Grid* attGrid, std::shared_ptr<InfoBar> bar, Coordinates coordinates, unsigned short int const number); // Explicit constructor : we can't create an implicit object in function parameters
	~Cell() = default; // We exprime the default destructor

	Cell(Cell const& other) = delete; // We delete the copy constructor
	void operator=(Cell const& other) = delete; // We delete the = operator to avoid object copies

	void updatePosition();
	void display(sf::RenderWindow& window) const;
	void implementMine();
	void incrementNumber();
	void changeFlag();
	void reveal();
	void revealNeighbourhood();
	void incrementNumberFlag(short int const number);
	void revealTruth();
	sf::FloatRect getBorder() const;

private:
	Game* game;
	Grid* grid;
	std::shared_ptr<InfoBar> infoBar;
	Coordinates coordinatesCell;
	unsigned short int numberColomns;

	float sizeSprite{};
	unsigned short int minesAround{ 0 };
	unsigned short int flagsAround{ 0 };
	bool hasMine{ false };
	bool isReveal{ false };
	bool hasFlag{ false };
	sf::Sprite sprite{};
	sf::Text text{};
	sf::Font font{};

	static constexpr std::string_view PATH_CELL_UNREVEAL{ "assets/cell_unreveal.png" };
	static constexpr std::string_view PATH_CELL_REVEAL{ "assets/cell_reveal.png" };
	static constexpr std::string_view PATH_CELL_FLAG{ "assets/cell_flag.png" };
	static constexpr std::string_view PATH_BOMB{ "assets/bomb.png" };
	static constexpr std::string_view PATH_WRONG_BOMB{ "assets/wrong_bomb.png" };
	static constexpr std::string_view PATH_FONT{ "assets/IMPACTED.ttf" };
	static constexpr float SIZE_TEXTURE{ 1024.f };
	static constexpr float TEXT_TRANSLATION_X{ 0.33f }; // Value used to correct the position of the text in the center of the cell (axis X)
	static constexpr float TEXT_TRANSLATION_Y{ -0.12f }; // Value used to correct the position of the text in the center of the cell (axis Y)

	static const std::array<sf::Color, 9> PERSONALIZED_COLORS;
};

#endif // !CELL_HPP
