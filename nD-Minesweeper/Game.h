#pragma once

#include "SFML/Graphics.hpp"
#include <memory>

class GameState;

class Game
{
private:
	sf::RenderWindow window;
	static std::unique_ptr<GameState> state;

	void run();
	void handleEvents();
	void update();
	void draw();
	void quit();

public:
	Game();
	static void setState(GameState* = nullptr);
};