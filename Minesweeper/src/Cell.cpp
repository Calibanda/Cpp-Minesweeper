#include "../include/Cell.hpp"
#include "../include/Grid.hpp"
#include "../include/Game.hpp"
#include "../include/Impacted.h"

using namespace std::string_literals; // Enable s-suffix for std::string literals

// Static fields must be initialised
const std::array<sf::Color, 9> Cell::PERSONALIZED_COLORS = std::array<sf::Color, 9>
{
	sf::Color(255, 255, 255), // White = number 0
	sf::Color(0, 0, 255), // Light blue = number 1
	sf::Color(0, 128, 0), // Green = number 2
	sf::Color(255, 0, 0), //Red = number 3
	sf::Color(0, 0, 128), // Dark blue = number 4
	sf::Color(128, 0, 0), // Dark red = number 5
	sf::Color(0, 128, 128), // Cyan = number 6
	sf::Color(128, 0, 128), // Magenta = number 7
	sf::Color(128, 128, 128) // Grey = number 8
};

Cell::Cell(Game* attGame, Grid* attGrid, std::shared_ptr<InfoBar> bar, Coordinates coordinates, unsigned short int const number) : game{ attGame }, grid{ attGrid }, infoBar{ bar }, coordinatesCell{ coordinates }, numberColomns{ number }
{
	sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_CELL_UNREVEAL));
	
	font.loadFromMemory(IMPACTED_ttf, IMPACTED_ttf_size);
	text.setFont(font);
	text.setString(""s);

	updatePosition();
}

void Cell::updatePosition()
{
	// Calculation of the scale applied on the sprite
	float scaleX = grid->getSizeGrid().x / (numberColomns * SIZE_TEXTURE);
	float scaleY = grid->getSizeGrid().y / (numberColomns * SIZE_TEXTURE);
	sprite.setScale(scaleX, scaleY);
	
	// Claculation and memorisation of the size of the sprite on screen
	sizeSprite = SIZE_TEXTURE * scaleX;

	sprite.setPosition(coordinatesCell.i * sizeSprite + grid->getMarginGrid().x, coordinatesCell.j * sizeSprite + grid->getMarginGrid().y);

	text.setPosition(sprite.getPosition().x + TEXT_TRANSLATION_X * sizeSprite, sprite.getPosition().y + TEXT_TRANSLATION_Y * sizeSprite);
	text.setCharacterSize(static_cast<int>(sizeSprite * 0.9));
}

void Cell::display(sf::RenderWindow& window) const
{
	window.draw(sprite);
	if (isReveal)
	{
		window.draw(text);
	}
}

void Cell::implementMine()
{
	//sprite.setColor(sf::Color::Red); // Use for debug
	text.setString(""s);
	hasMine = true;
}

void Cell::incrementNumber()
{
	if (!hasMine)
	{
		minesAround++;
		text.setString(std::to_string(minesAround));
		text.setFillColor(PERSONALIZED_COLORS[minesAround]);
	}
}

void Cell::changeFlag()
{
	if (!isReveal)
	{
		hasFlag = !hasFlag;
		infoBar->startChrono();
		if (hasFlag)
		{
			sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_CELL_FLAG));
			for (short int i = -1; i < 2; i++)
			{
				for (short int j = -1; j < 2; j++)
				{
					if (coordinatesCell.i + i >= 0 && coordinatesCell.i + i < numberColomns && coordinatesCell.j + j >= 0 && coordinatesCell.j + j < numberColomns && !(i == 0 && j == 0))
					{
						grid->incrementNumberFlagCell(coordinatesCell.i + i, coordinatesCell.j + j, 1); // All the cells around increment their flag number
					}
				}
			}

			infoBar->incrementNumberFlags(1);
		}
		else
		{
			sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_CELL_UNREVEAL));
			for (short int i = -1; i < 2; i++)
			{
				for (short int j = -1; j < 2; j++)
				{
					if (coordinatesCell.i + i >= 0 && coordinatesCell.i + i < numberColomns && coordinatesCell.j + j >= 0 && coordinatesCell.j + j < numberColomns && !(i == 0 && j == 0))
					{
						grid->incrementNumberFlagCell(coordinatesCell.i + i, coordinatesCell.j + j, -1); // All the cells around decrement their flag number
					}
				}
			}

			infoBar->incrementNumberFlags(-1);
		}
	}
}

void Cell::reveal()
{
	if (!isReveal && !hasFlag)
	{
		isReveal = true;
		infoBar->startChrono();

		if (hasMine)
		{
			sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_BOMB));
			sprite.setColor(sf::Color::Red);
			grid->makeExplosion(static_cast<unsigned short int>(sprite.getPosition().x + sizeSprite / 2), static_cast<unsigned short int>(sprite.getPosition().y + sizeSprite / 2));
			game->gameOver(false);
		}
		else
		{
			sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_CELL_REVEAL));
			grid->incrementRevealedCells();

			if (minesAround == 0) // If the cell has no mines around we need to discover all its neighbours
			{
				for (short int i = -1; i < 2; i++)
				{
					for (short int j = -1; j < 2; j++)
					{
						if (coordinatesCell.i + i >= 0 && coordinatesCell.i + i < numberColomns && coordinatesCell.j + j >= 0 && coordinatesCell.j + j < numberColomns && !(i == 0 && j == 0))
						{
							grid->revealCell(coordinatesCell.i + i, coordinatesCell.j + j);
						}
					}
				}
			}
		}
	}
}

void Cell::revealNeighbourhood()
{
	if (isReveal && flagsAround == minesAround)
	{
		for (short int i = -1; i < 2; i++)
		{
			for (short int j = -1; j < 2; j++)
			{
				if (coordinatesCell.i + i >= 0 && coordinatesCell.i + i < numberColomns && coordinatesCell.j + j >= 0 && coordinatesCell.j + j < numberColomns && !(i == 0 && j == 0))
				{
					grid->revealCell(coordinatesCell.i + i, coordinatesCell.j + j);
				}
			}
		}
	}
}

void Cell::incrementNumberFlag(short int const number)
{
	flagsAround += number;
}

void Cell::revealTruth()
{
	if (!isReveal && hasMine && !hasFlag)
	{
		sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_BOMB));
	}
	if (hasFlag && !hasMine)
	{
		sprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_WRONG_BOMB));
	}
}

sf::FloatRect Cell::getBorder() const
{
	return sprite.getGlobalBounds();
}
