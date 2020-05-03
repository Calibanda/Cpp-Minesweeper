// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <exception>
#include "include/Game.hpp"

constexpr unsigned int MIN_SIZE_WINDOW{ 500 };

int main()
{
    sf::RenderWindow myWindow{ sf::VideoMode(800, 800), "Minesweeper" }; // Declaration and initialisation of a new RenderWindow object
    Game minesweeper = Game(myWindow.getSize()); // Instanciation of a new Game object
    
    try
    {
        while (myWindow.isOpen())
        {
            sf::Event myEvent = sf::Event(); // Declaration of a new event object
            while (myWindow.pollEvent(myEvent)) // While the window has user events to evaluate, we process thoses events
            {
                if (myEvent.type == sf::Event::Closed)
                {
                    // We close the window
                    myWindow.close();
                    break; // We don't want process any event for this frame
                }
                if (myEvent.type == sf::Event::Resized)
                {
                    // We update the view with the new window size either the new size or the minimum size
                    unsigned int newWidth = MIN_SIZE_WINDOW;
                    unsigned int newHeight = MIN_SIZE_WINDOW;

                    if (myEvent.size.width > MIN_SIZE_WINDOW)
                    {
                        newWidth = myEvent.size.width;
                    }
                    if (myEvent.size.height > MIN_SIZE_WINDOW)
                    {
                        newHeight = myEvent.size.height;
                    }

                    myWindow.setSize(sf::Vector2u(newWidth, newHeight)); // We resize the window
                    myWindow.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(newWidth), static_cast<float>(newHeight)))); // We correctly redefine the view
                    minesweeper.updateSize(myWindow.getSize()); // We ask the game to correct all the elements' positions
                }
                else if ((myEvent.type == sf::Event::KeyPressed || myEvent.type == sf::Event::MouseButtonPressed) && minesweeper.isEnding())
                {
                    // If the ending screen is display we end the current game and display the menu
                    minesweeper.end();
                    break; // We don't want process any event for this frame
                }
                else if (sf::Mouse::getPosition(myWindow).x >= 0 && sf::Mouse::getPosition(myWindow).y >= 0 && sf::Mouse::getPosition(myWindow).x <= static_cast<int>(myWindow.getSize().x) && sf::Mouse::getPosition(myWindow).y <= static_cast<int>(myWindow.getSize().y) && (myEvent.type == sf::Event::MouseButtonPressed || myEvent.type == sf::Event::MouseButtonReleased))
                {
                    // If the game is running and the mouse is in the window and the right or the left click is pressed
                    minesweeper.detectClicks(myEvent, myWindow);
                }
            }

            minesweeper.update(myWindow); // We update the game at each frame

            myWindow.clear(); // Clean the main window to display the next frame
            minesweeper.display(myWindow); // We draw all the elements on the main window
            myWindow.display(); // The window is display to the user while the window is open
        }
    }
    catch (std::exception const& exception)
    {
        minesweeper.initialiseException(exception);
    }

    return EXIT_SUCCESS;
}
