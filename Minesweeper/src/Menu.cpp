#include "../include/Menu.hpp"
#include "../include/Game.hpp"
#include "../include/Impacted.h"
#include <string>

using namespace std::string_literals;

Menu::Menu(Game* ptrGame, Size size) : game{ ptrGame }, sizeWindow{ size }
{
	if (!font.loadFromMemory(IMPACTED_ttf, IMPACTED_ttf_size)) // Loard from a .h font
	{
		throw std::runtime_error("Can not load font");
	}

	textEasy.setFont(font);
	textEasy.setFillColor(COLOR_TEXT);
	textEasy.setCharacterSize(60);
	textMedium.setFont(font);
	textMedium.setFillColor(COLOR_TEXT);
	textMedium.setCharacterSize(60);
	textHard.setFont(font);
	textHard.setFillColor(COLOR_TEXT);
	textHard.setCharacterSize(60);
}

void Menu::updateTextDisplay(Size const& newSize, std::vector<unsigned int> const& bestTimes)
{
	sizeWindow.x = newSize.x;
	sizeWindow.y = newSize.y;

	if (bestTimes[0] != 0)
	{
		std::string minutes = std::to_string(bestTimes[0] / 60);
		std::string seconds = std::to_string(bestTimes[0] % 60);

		if (seconds.size() < 2)
		{
			seconds = "0"s + seconds;
		}
		textEasy.setString("Easy - Best time: "s + minutes + ":"s + seconds);
	}
	else
	{
		textEasy.setString("Easy"s);
	}
	textEasy.setOrigin(textEasy.getLocalBounds().width / 2, textEasy.getLocalBounds().height / 2);
	textEasy.setPosition(sizeWindow.x * RELATIVE_POSITION_TEXT_EASY_X, sizeWindow.y * RELATIVE_POSITION_TEXT_EASY_Y);

	if (bestTimes[1] != 0)
	{
		std::string minutes = std::to_string(bestTimes[1] / 60);
		std::string seconds = std::to_string(bestTimes[1] % 60);

		if (seconds.size() < 2)
		{
			seconds = "0"s + seconds;
		}
		textMedium.setString("Medium - Best time: "s + minutes + ":"s + seconds);
	}
	else
	{
		textMedium.setString("Medium"s);
	}
	textMedium.setOrigin(textMedium.getLocalBounds().width / 2, textMedium.getLocalBounds().height / 2);
	textMedium.setPosition(sizeWindow.x * RELATIVE_POSITION_TEXT_MEDIUM_X, sizeWindow.y * RELATIVE_POSITION_TEXT_MEDIUM_Y);

	if (bestTimes[2] != 0)
	{
		std::string minutes = std::to_string(bestTimes[2] / 60);
		std::string seconds = std::to_string(bestTimes[2] % 60);

		if (seconds.size() < 2)
		{
			seconds = "0"s + seconds;
		}
		textHard.setString("Hard - Best time: "s + minutes + ":"s + seconds);
	}
	else
	{
		textHard.setString("Hard"s);
	}
	textHard.setOrigin(textHard.getLocalBounds().width / 2, textHard.getLocalBounds().height / 2);
	textHard.setPosition(sizeWindow.x * RELATIVE_POSITION_TEXT_HARD_X, sizeWindow.y * RELATIVE_POSITION_TEXT_HARD_Y);
}

void Menu::mouseLeftPress(sf::Vector2i const& mousePosition)
{
	if (textEasy.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		lastPressedButton = Difficulty::EASY;
	}
	else if(textMedium.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		lastPressedButton = Difficulty::MEDIUM;
	}
	else if (textHard.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		lastPressedButton = Difficulty::HARD;
	}

}

void Menu::mouseLeftRelease(sf::Vector2i const& mousePosition)
{
	if (lastPressedButton == Difficulty::EASY && textEasy.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		game->start(Difficulty::EASY);
	}
	else if (lastPressedButton == Difficulty::MEDIUM && textMedium.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		game->start(Difficulty::MEDIUM);
	}
	else if (lastPressedButton == Difficulty::HARD && textHard.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		game->start(Difficulty::HARD);
	}

	lastPressedButton = Difficulty::NONE;
}

void Menu::updateMouseInfo(sf::RenderWindow& window)
{
	textEasy.setFillColor(COLOR_TEXT);
	textMedium.setFillColor(COLOR_TEXT);
	textHard.setFillColor(COLOR_TEXT);

	if (textEasy.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
	{
		textEasy.setFillColor(sf::Color::Green);
	}
	else if (textMedium.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
	{
		textMedium.setFillColor(sf::Color(255, 128, 0));
	}
	else if (textHard.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
	{
		textHard.setFillColor(sf::Color::Red);
	}
}

void Menu::display(sf::RenderWindow& window)
{
	window.draw(textEasy);
	window.draw(textMedium);
	window.draw(textHard);
}
