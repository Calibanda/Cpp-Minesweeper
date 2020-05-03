#ifndef GAME_HPP
#define GAME_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <string_view>
#include <exception>
#include "Grid.hpp"
#include "Size.hpp"
#include "Menu.hpp"
#include "InfoBar.hpp"

class Game
{
public:
	explicit Game(sf::Vector2u const& size); // Explicit constructor : we can't create an implicit object in function parameters
	~Game() = default; // We exprime the default virtual destructor in oder to call the right subclass destructor

	Game(Game const& other) = delete; //We delete the copy constructor
	void operator=(Game const& other) = delete; //We delete the = operator to avoid object copies

	void updateSize(sf::Vector2u const& newSize); // Correct all the positions of the existing elements
	void start(Difficulty const difficulty);
	void gameOver(bool win);
	void end();
	void display(sf::RenderWindow& window); // Display all the visible elements on the window
	void detectClicks(sf::Event const& mouseEvent, sf::RenderWindow& window); // Send mouse inputs to the concerned elements
	void update(sf::RenderWindow& window); // Update all the existing elements
	bool isRunning() const;
	bool isEnding() const;
	void initialiseException(std::exception const& exception);

private:
	Size sizeWindow;

	std::unique_ptr<Grid> grid{};
	std::shared_ptr<InfoBar> infoBar{};
	std::unique_ptr<Menu> menu{};

	sf::Sprite welcomeSprite{};
	sf::Font font{};
	Difficulty currentDifficulty{ Difficulty::NONE };
	unsigned int score{};
	std::vector<unsigned int> bestTimes{ 0, 0, 0 };
	sf::Mouse::Button lastPressedButton{};
	sf::Clock timeBetweenLeftPress{};
	sf::Clock endTimer{};
	sf::Music music{};
	sf::Text endingText{};
	std::unique_ptr<sf::Text> textException{ nullptr };
	bool running{ false };
	bool gameIsOver{ false };
	bool musicIsOpen{ false };

	static constexpr std::string_view PATH_WELCOME{ "assets/welcome.png" };
	static constexpr std::string_view PATH_FILE_SCORE{ "best.score" };
	static constexpr std::string_view PATH_MUSIC{ "assets/El_Sonidito.wav" };
	static constexpr unsigned short int HEIGHT_INFO_BAR{ 100 };
	static constexpr float SIZE_WELCOME{ 1024.f };
	static constexpr float DELTA_TIME_DOUBLE_CLICK{ 500 };
};

#endif // !GAME_HPP
