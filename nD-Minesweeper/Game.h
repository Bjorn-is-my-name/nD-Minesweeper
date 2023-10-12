#pragma once

#include "SFML/Graphics.hpp"

// Base game class, page and event handler
class Game
{
private:
	sf::RenderWindow window;					// Main application window

	void run();									// Game loop
	void handleEvents();						// Eventhandler
	void update();								// Update things (executed every frame)
	void draw();								// Draw things (executes every frame)
	void quit();								// Closes application

public:
	Game();
};