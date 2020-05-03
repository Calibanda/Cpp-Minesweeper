#include "../include/InfoBar.hpp"
#include "../include/Impacted.h"
#include <string>

using namespace std::string_literals; // Enable s-suffix for std::string literals

InfoBar::InfoBar(Game* ptrGame, Size size, unsigned short int const height, unsigned short int const mines) : game{ ptrGame }, sizeWindow{ size }, height{ height }, nbMines{ mines }
{
	font.loadFromMemory(IMPACTED_ttf, IMPACTED_ttf_size);

	textChronometer.setFont(font);
	textChronometer.setString("00:00"s);
	textChronometer.setCharacterSize(80);
	textChronometer.setFillColor(COLOR_TEXT);

	textChronometer.setOrigin(0.f, textChronometer.getLocalBounds().height);
	textChronometer.setPosition(static_cast<float>(MARGIN_X), static_cast<float>(sizeWindow.y - MARGIN_Y));

	minesCounter.setFont(font);
	minesCounter.setString(std::to_string(nbFlags) + "/"s + std::to_string(nbMines));
	minesCounter.setCharacterSize(80);
	minesCounter.setFillColor(COLOR_TEXT);

	minesCounter.setOrigin(minesCounter.getLocalBounds().width, minesCounter.getLocalBounds().height);
	minesCounter.setPosition(static_cast<float>(sizeWindow.x - MARGIN_X), static_cast<float>(sizeWindow.y - MARGIN_Y));
}

void InfoBar::updateSize(Size const& newSize)
{
	sizeWindow = newSize;
	textChronometer.setPosition(static_cast<float>(MARGIN_X), static_cast<float>(sizeWindow.y - MARGIN_Y));
	minesCounter.setPosition(static_cast<float>(sizeWindow.x - MARGIN_X), static_cast<float>(sizeWindow.y - MARGIN_Y));
}

void InfoBar::incrementNumberFlags(short int const incrementNumber)
{
	nbFlags += incrementNumber;
	minesCounter.setString(std::to_string(nbFlags) + "/"s + std::to_string(nbMines));
	minesCounter.setOrigin(minesCounter.getLocalBounds().width, minesCounter.getOrigin().y);
	minesCounter.setPosition(static_cast<float>(sizeWindow.x - MARGIN_X), static_cast<float>(sizeWindow.y - MARGIN_Y));
}

void InfoBar::startChrono()
{
	if (!chronoIsStarted && !chronoIsStoped)
	{
		chrono.restart();
		chronoIsStarted = true;
	}
}

unsigned int InfoBar::stopChrono()
{
	chronoIsStoped = true;
	return time;
}

void InfoBar::update(sf::RenderWindow& window)
{
	if (chronoIsStarted && !chronoIsStoped)
	{
		time = static_cast<unsigned int>(chrono.getElapsedTime().asSeconds());
		std::string minutes = std::to_string(time / 60);
		std::string seconds = std::to_string(time % 60);

		if (minutes.size() < 2)
		{
			minutes = "0"s + minutes;
		}
		if (seconds.size() < 2)
		{
			seconds = "0"s + seconds;
		}

		textChronometer.setString(minutes + ":"s + seconds);
	}
}

void InfoBar::display(sf::RenderWindow& window)
{
	window.draw(minesCounter);
	window.draw(textChronometer);
}
