#pragma once

#include "SFML/Graphics.hpp"
#include <memory>

// Pre initialize the gamestate class (partial class) to prevent circular dependency by having to include 'GameState.h'
class GameState;

// Base game class, page and event handler
class Game
{
private:
	sf::RenderWindow window;					// Main application window
	static std::unique_ptr<GameState> state;	// Current gamestate

	void run();									// Game loop
	void handleEvents();						// Eventhandler
	void update();								// Update things (executed every frame)
	void draw();								// Draw things (executes every frame)
	void quit();								// Closes application

public:
	Game();
	static void setState(GameState* = nullptr); // Changes the game state
};