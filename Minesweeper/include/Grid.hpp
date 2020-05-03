#ifndef GRID_HPP
#define GRID_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Cell.hpp"
#include "Explosion.hpp"
#include "Size.hpp"
#include "InfoBar.hpp"
#include "Coordinates.hpp"

class Game; // We can't include "Game.hpp" (recursive definition) so we use a forward declaration and include "Game.hpp" in "Grid.cpp"

enum class MouseEvent { LEFT_PRESS, LEFT_RELEASE, RIGHT_PRESS, RIGHT_RELEASE, DOUBLE_LEFT_CLICK };

class Grid
{
public:
	explicit Grid(Game* attGame, Size size, Size margin, std::shared_ptr<InfoBar> bar, unsigned short int const nbCells, unsigned short int const nbMines); // Explicit constructor : we can't create an implicit object in function parameters
	~Grid() = default; // We exprime the default destructor

	Grid(Grid const& other) = delete; // We delete the copy constructor
	void operator=(Grid const& other) = delete; // We delete the = operator to avoid object copies

	void updateSize(Size const& newSize, Size const& margin); // Update the position of the cells in the new window
	void manageMouseEvent(sf::Vector2i const& mousePosition, MouseEvent mouseAction);
	void revealCell(unsigned short int const positionCellI, unsigned short int const positionCellJ);
	void incrementNumberFlagCell(unsigned short int const positionCellI, unsigned short int const positionCellJ, short int const incrementNumber);
	void revealCellNeighbourhood(unsigned short int const positionCellI, unsigned short int const positionCellJ);
	void incrementRevealedCells();
	void makeExplosion(unsigned short int const posX, unsigned short int const posY);
	void truthAboutCells();
	void updateMouseInfo(sf::RenderWindow& window);
	void display(sf::RenderWindow& window);
	Size getSizeGrid() const;
	Size getMarginGrid() const;

private:
	Game* game;
	Size sizeGrid;
	Size marginGrid;
	std::shared_ptr<InfoBar> infoBar;
	unsigned short int numberColumn;
	unsigned short int numberMines;

	short int numberFlags{ 0 };
	unsigned short int revealedCells{ 0 };
	std::vector<Coordinates> locationMines{}; // Vector containing the coordinates of the mines
	std::unique_ptr<Coordinates> lastCellLeftPressed{ nullptr };
	std::unique_ptr<Coordinates> lastCellRightPressed{ nullptr };
	std::vector<std::vector<std::unique_ptr<Cell>>> elements{}; // Two dimentional vector that contains all the cells
	std::unique_ptr<Explosion> explosion{ nullptr };
};

#endif // !GRID_HPP
