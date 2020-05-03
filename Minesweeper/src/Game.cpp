#include "../include/Game.hpp"
#include "../include/Cell.hpp"
#include "../include/ResourcesManager.hpp"
#include "../include/Impacted.h"
#include <string>
#include <fstream> // Read and write in files
#include <algorithm> // std::min()

using namespace std::string_literals; // Enable s-suffix for std::string literals

Game::Game(sf::Vector2u const& size) : sizeWindow{ size.x, size.y }
{
	if (!font.loadFromMemory(IMPACTED_ttf, IMPACTED_ttf_size)) // Loard from a .h font
	{
		throw std::runtime_error("Can not load font");
	}

	endingText.setFont(font);
	endingText.setOutlineColor(sf::Color::Black);
	endingText.setOutlineThickness(5.f);
	endingText.setCharacterSize(140);

	try
	{
		welcomeSprite.setTexture(ResourcesManager<sf::Texture>::getResource(PATH_WELCOME));
		welcomeSprite.setScale(std::min(sizeWindow.x / SIZE_WELCOME, sizeWindow.y / SIZE_WELCOME), std::min(sizeWindow.x / SIZE_WELCOME, sizeWindow.y / SIZE_WELCOME));
		welcomeSprite.setOrigin(welcomeSprite.getLocalBounds().width / 2, welcomeSprite.getLocalBounds().height / 2);
		welcomeSprite.setPosition(sizeWindow.x / 2.f, sizeWindow.y / 2.f);
	}
	catch (std::exception const& exception)
	{
		initialiseException(exception);
	}

	if (musicIsOpen = music.openFromFile(PATH_MUSIC.data()))
	{
		music.setLoop(true);
		music.setVolume(50.f);
		//music.setVolume(0.f); // Use for debug
		music.setPitch(1.5f);
	}

	std::ifstream fileBestScore = std::ifstream(std::string(PATH_FILE_SCORE).c_str(), std::ios::in | std::ios::binary); // Opening best.score in binary
	if (fileBestScore.is_open())
	{
		for (unsigned short int i = 0; i < 3; i++)
		{
			fileBestScore.read((char*)&bestTimes[i], sizeof(unsigned int));
		}
	}
	fileBestScore.close();

	menu = std::make_unique<Menu>(this, sizeWindow);
	menu->updateTextDisplay(sizeWindow, bestTimes);
}

void Game::updateSize(sf::Vector2u const& newSize)
{
	// Correct all the positions of the existing elements
	sizeWindow.x = newSize.x;
	sizeWindow.y = newSize.y;

	welcomeSprite.setScale(std::min(sizeWindow.x / SIZE_WELCOME, sizeWindow.y / SIZE_WELCOME), std::min(sizeWindow.x / SIZE_WELCOME, sizeWindow.y / SIZE_WELCOME));
	welcomeSprite.setOrigin(welcomeSprite.getLocalBounds().width / 2, welcomeSprite.getLocalBounds().height / 2);
	welcomeSprite.setPosition(sizeWindow.x / 2.f, sizeWindow.y / 2.f);

	menu->updateTextDisplay(sizeWindow, bestTimes);

	if (running)
	{
		Size sizeGrid = Size(std::min(static_cast<int>(sizeWindow.x), sizeWindow.y - HEIGHT_INFO_BAR), std::min(static_cast<int>(sizeWindow.x), sizeWindow.y - HEIGHT_INFO_BAR));
		Size marginGrid = Size((sizeWindow.x - sizeGrid.x) / 2, (sizeWindow.y - HEIGHT_INFO_BAR - sizeGrid.y) / 2);
		grid->updateSize(sizeGrid, marginGrid);

		infoBar->updateSize(sizeWindow);

		if (gameIsOver)
		{
			endingText.setPosition(sizeWindow.x / 2.f, sizeWindow.y / 2.f);
		}
	}
}

void Game::start(Difficulty const difficulty)
{
	currentDifficulty = difficulty;
	unsigned short int nbColomn;
	unsigned short int nbMines;
	gameIsOver = false;
	running = true;

	Size sizeGrid = Size(std::min(static_cast<int>(sizeWindow.x), sizeWindow.y - HEIGHT_INFO_BAR), std::min(static_cast<int>(sizeWindow.x), sizeWindow.y - HEIGHT_INFO_BAR));
	Size marginGrid = Size((sizeWindow.x - sizeGrid.x) / 2, (sizeWindow.y - HEIGHT_INFO_BAR - sizeGrid.y) / 2);

	if (currentDifficulty == Difficulty::EASY)
	{
		nbColomn = 8;
		nbMines = 10;
	}
	else if (currentDifficulty == Difficulty::MEDIUM)
	{
		nbColomn = 10;
		nbMines = 20;
	}
	else if (currentDifficulty == Difficulty::HARD)
	{
		nbColomn = 12;
		nbMines = 30;
	}

	infoBar = std::make_unique<InfoBar>(this, sizeWindow, HEIGHT_INFO_BAR, nbMines); // Creation of the infobar
	grid = std::make_unique<Grid>(this, sizeGrid, marginGrid, infoBar, nbColomn, nbMines); // Creation of the grid
}

void Game::gameOver(bool win)
{
	gameIsOver = true;
	score = infoBar->stopChrono();
	grid->truthAboutCells();
	if (win)
	{
		endingText.setString("You win!"s);
		endingText.setFillColor(sf::Color::Green);
	}
	else
	{
		endingText.setString("You lost!"s);
		endingText.setFillColor(sf::Color::Red);
		score = 0;
	}

	endingText.setOrigin(endingText.getLocalBounds().width / 2, endingText.getLocalBounds().height);
	endingText.setPosition(sizeWindow.x / 2.f, sizeWindow.y / 2.f);
	endTimer.restart();
}

void Game::end()
{
	if (endTimer.getElapsedTime().asSeconds() > 0.5f)
	{
		grid = nullptr;
		infoBar = nullptr;
		gameIsOver = false;
		running = false;

		if (currentDifficulty == Difficulty::EASY)
		{
			if (bestTimes[0] == 0 || (score < bestTimes[0] && score != 0))
			{
				bestTimes[0] = score;
			}
		}
		else if (currentDifficulty == Difficulty::MEDIUM)
		{
			if (bestTimes[1] == 0 || (score < bestTimes[1] && score != 0))
			{
				bestTimes[1] = score;
			}
		}
		else if (currentDifficulty == Difficulty::HARD)
		{
			if (bestTimes[2] == 0 || (score < bestTimes[2] && score != 0))
			{
				bestTimes[2] = score;
			}
		}

		std::ofstream fileBestScore = std::ofstream(std::string(PATH_FILE_SCORE).c_str(), std::ios::out | std::ios::binary);
		if (fileBestScore.is_open())
		{
			for (unsigned short int i = 0; i < 3; i++)
			{
				fileBestScore.write((char*)&bestTimes[i], sizeof(unsigned int)); //Write in a binary document
			}
		}
		else
		{
			throw std::runtime_error("Failed to load best score file in writing");
		}
		fileBestScore.close();

		menu->updateTextDisplay(sizeWindow, bestTimes);
	}
}

void Game::display(sf::RenderWindow& window)
{
	// Display all the visible elements on the window
	if (textException)
	{
		window.draw(*textException);
	}
	else
	{
		if (musicIsOpen && music.getStatus() == sf::SoundSource::Status::Stopped)
		{
			music.play();
		}

		if(running)
		{
			grid->display(window);
			infoBar->display(window);

			if (gameIsOver)
			{
				window.draw(endingText);
			}
		}
		else
		{
			window.draw(welcomeSprite);
			menu->display(window);
		}
	}
}

void Game::detectClicks(sf::Event const& mouseEvent, sf::RenderWindow& window)
{
	// Send mouse inputs to the concerned elements
	if (running)
	{
		if (mouseEvent.type == sf::Event::MouseButtonPressed)
		{
			if (mouseEvent.mouseButton.button == sf::Mouse::Left)
			{
				if (lastPressedButton == sf::Mouse::Button::Left && timeBetweenLeftPress.getElapsedTime().asMilliseconds() <= DELTA_TIME_DOUBLE_CLICK)
				{	
					// If double click
					grid->manageMouseEvent(sf::Mouse::getPosition(window), MouseEvent::DOUBLE_LEFT_CLICK);
				}

				grid->manageMouseEvent(sf::Mouse::getPosition(window), MouseEvent::LEFT_PRESS);
				lastPressedButton = sf::Mouse::Button::Left;
				timeBetweenLeftPress.restart();

			}
			else if (mouseEvent.mouseButton.button == sf::Mouse::Right)
			{
				grid->manageMouseEvent(sf::Mouse::getPosition(window), MouseEvent::RIGHT_PRESS);
				lastPressedButton = sf::Mouse::Button::Right;
			}
		}
		else if (mouseEvent.type == sf::Event::MouseButtonReleased)
		{
			if (mouseEvent.mouseButton.button == sf::Mouse::Left)
			{
				grid->manageMouseEvent(sf::Mouse::getPosition(window), MouseEvent::LEFT_RELEASE);
			}
			else if (mouseEvent.mouseButton.button == sf::Mouse::Right)
			{
				grid->manageMouseEvent(sf::Mouse::getPosition(window), MouseEvent::RIGHT_RELEASE);
			}
		}
	}
	else
	{
		if(mouseEvent.type == sf::Event::MouseButtonPressed && mouseEvent.mouseButton.button == sf::Mouse::Left)
		{
			menu->mouseLeftPress(sf::Mouse::getPosition(window));
		}
		else if (mouseEvent.type == sf::Event::MouseButtonReleased && mouseEvent.mouseButton.button == sf::Mouse::Left)
		{
			menu->mouseLeftRelease(sf::Mouse::getPosition(window));
		}
	}
}

void Game::update(sf::RenderWindow& window)
{
	// Update all the existing elements
	if (running)
	{
		grid->updateMouseInfo(window);
		infoBar->update(window);
	}
	else
	{
		menu->updateMouseInfo(window);
	}
}

bool Game::isRunning() const { return running; }

bool Game::isEnding() const { return gameIsOver; }

void Game::initialiseException(std::exception const& exception)
{
	textException = std::make_unique<sf::Text>();
	textException->setFont(font);
	textException->setString(exception.what());
	textException->setFillColor(sf::Color::Red);
}
