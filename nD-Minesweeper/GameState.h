#pragma once

#include "Game.h"

class GameState
{
public:
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void keyPressed(const sf::Keyboard::Key key) = 0;
	virtual void mousePressed(const sf::Event::MouseButtonEvent event) = 0;
	virtual void mouseReleased(const sf::Event::MouseButtonEvent event) = 0;
};