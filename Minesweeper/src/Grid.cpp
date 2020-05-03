#include <random>
#include <ctime> // time
#include "../include/Grid.hpp"
#include "../include/Game.hpp"

Grid::Grid(Game* attGame, Size size, Size margin, std::shared_ptr<InfoBar> bar, unsigned short int const nbColumn, unsigned short int const nbMines) : game{ attGame }, sizeGrid{ size }, marginGrid{ margin }, infoBar{ bar }, numberColumn{ nbColumn }, numberMines{ nbMines }
{
    // Creation of the cells
    for (unsigned short int i = 0; i < numberColumn; i++)
    {
        elements.push_back(std::vector<std::unique_ptr<Cell>>()); // Add all empty rows
    }

    for (unsigned short int j = 0; j < numberColumn; j++)
    {
        for (unsigned short int i = 0; i < numberColumn; i++)
        {
            elements[i].push_back(std::make_unique<Cell>(game, this, infoBar, Coordinates(i, j), numberColumn)); // Add all cells
        }
    }

    // Implementation of the mines
    std::mt19937 generator; // Declaration of our random number generator

    try
    {
        generator = std::mt19937{ std::random_device{}() }; // Creation of a random generator. Seed = full random number
    }
    catch (const std::exception&) // Catch a posible exception in case std::random_device is not available on the system
    {
        generator = std::mt19937{ (unsigned int)time(nullptr) }; // Creation of a random generator. Seed = time
    }

    std::uniform_int_distribution<int> distributionMines = std::uniform_int_distribution<int>(0, numberColumn - 1); // We set the distribution to have numbers between the first and the last cell

    for (unsigned short int i = 0; i < numberMines; i++) // We want to create several mines and memorize them in 'locationMines'
    {
        bool isUnique{ false };
        while (!isUnique) // We want to check the uniqueness of the coordinates
        {
            Coordinates coordinates = Coordinates(distributionMines(generator), distributionMines(generator)); // We create a couple of number
            isUnique =  true;
            for (unsigned short int i = 0; i < locationMines.size(); i++) // We iterate the previous coordinates to check if the new couple is unique
            {
                if (locationMines[i].i == coordinates.i && locationMines[i].j == coordinates.j) // If the 2 coordinates are the same
                {
                    isUnique = false;
                }
            }
            if (isUnique)
            {
                locationMines.push_back(coordinates);
            }
        }
    }

    // 'locationMines' is now complete and we will iterate it to set the mines in the cells
    for (unsigned short int i = 0; i < locationMines.size(); i++)
    {
        // For each coordinates stocked in the vector, we put a mine in the cell
        elements[locationMines[i].i][locationMines[i].j]->implementMine();

        // We want increse the number of each cell around the coordinates.
        for (short int j = -1; j < 2; j++)
        {
            for (short int k = -1; k < 2; k++)
            {
                if (locationMines[i].i + j >= 0 && locationMines[i].i + j < numberColumn && locationMines[i].j + k >= 0 && locationMines[i].j + k < numberColumn && !(j == 0 && k == 0))
                {
                    // We first check those coordinate exist before access it.
                    elements[locationMines[i].i + j][locationMines[i].j + k]->incrementNumber();
                }
            }
        }
    }
}

void Grid::updateSize(Size const& newSize, Size const& margin)
{
    // Update the position of the cells in the new window

    sizeGrid = newSize;
    marginGrid = margin;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        for (unsigned int j = 0; j < elements[i].size(); j++)
        {
            elements[i][j]->updatePosition();
        }
    }
}

void Grid::manageMouseEvent(sf::Vector2i const& mousePosition, MouseEvent mouseAction)
{
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        for (unsigned int j = 0; j < elements[i].size(); j++)
        {
            if (elements[i][j]->getBorder().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
            {
                // If the mouse position is in the cell borders
                if (mouseAction == MouseEvent::LEFT_PRESS)
                {
                    lastCellLeftPressed = std::make_unique<Coordinates>(i, j); // We create the coordinate for the last pressed cell
                }
                else if (mouseAction == MouseEvent::RIGHT_PRESS)
                {
                    lastCellRightPressed = std::make_unique<Coordinates>(i, j); // We create the coordinate for the last pressed cell
                }
                else if (mouseAction == MouseEvent::LEFT_RELEASE && lastCellLeftPressed && lastCellLeftPressed->i == i && lastCellLeftPressed->j == j)
                {
                    // If the mouse has been pressed above the same cell than released
                    elements[i][j]->reveal();
                }
                else if (mouseAction == MouseEvent::RIGHT_RELEASE && lastCellRightPressed && lastCellRightPressed->i == i && lastCellRightPressed->j == j)
                {
                    // If the mouse has been pressed above the same cell than released
                    elements[i][j]->changeFlag();
                }
                else if (mouseAction == MouseEvent::DOUBLE_LEFT_CLICK && lastCellLeftPressed && lastCellLeftPressed->i == i && lastCellLeftPressed->j == j)
                {
                    // If the mouse has been pressed above the same cell than released
                    elements[i][j]->revealNeighbourhood();
                }

                return; // We found the cell, we exit the function
            }
        }
    }
}

void Grid::revealCell(unsigned short int const positionCellI, unsigned short int const positionCellJ)
{
    elements[positionCellI][positionCellJ]->reveal();
}

void Grid::incrementNumberFlagCell(unsigned short int const positionCellI, unsigned short int const positionCellJ, short int const incrementNumber)
{
    elements[positionCellI][positionCellJ]->incrementNumberFlag(incrementNumber);
}

void Grid::revealCellNeighbourhood(unsigned short int const positionCellI, unsigned short int const positionCellJ)
{
    elements[positionCellI][positionCellJ]->revealNeighbourhood();
}

void Grid::incrementRevealedCells()
{
    revealedCells++;
    if (revealedCells == numberColumn * numberColumn - numberMines)
    {
        // All the safe cells has been revealed
        game->gameOver(true);
    }
}

void Grid::makeExplosion(unsigned short int const posX, unsigned short int const posY)
{
    explosion = std::make_unique<Explosion>(posX, posY);
}

void Grid::truthAboutCells()
{
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        for (unsigned int j = 0; j < elements[i].size(); j++)
        {
            elements[i][j]->revealTruth();
        }
    }
}

void Grid::updateMouseInfo(sf::RenderWindow& window)
{
    if (lastCellLeftPressed)
    {
        if (!elements[lastCellLeftPressed->i][lastCellLeftPressed->j]->getBorder().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
        {
            // If the last pressed element is no longer above the same cell
            lastCellLeftPressed = nullptr;
        }
    }

    if (lastCellRightPressed)
    {
        if (!elements[lastCellRightPressed->i][lastCellRightPressed->j]->getBorder().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
        {
            // If the last pressed element is no longer above the same cell
            lastCellRightPressed = nullptr;
        }
    }
}

void Grid::display(sf::RenderWindow& window)
{
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        for (unsigned int j = 0; j < elements[i].size(); j++)
        {
            elements[i][j]->display(window);
        }
    }

    if (explosion)
    {
        explosion->display(window);
        if (explosion->isFinished())
        {
            explosion = nullptr; // Destroy object
        }
    }
}

Size Grid::getSizeGrid() const
{
    return sizeGrid;
}

Size Grid::getMarginGrid() const
{
    return marginGrid;
}
