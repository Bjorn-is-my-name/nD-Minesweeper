#pragma once

#include "Game.h"

// Interface for the gamestates
class GameState
{
public:
	virtual void update() = 0;													// Updates the game
	virtual void draw(sf::RenderWindow& window) = 0;							// Draws all the elements
	virtual void keyPressed(const sf::Keyboard::Key& key) = 0;					// Handles key presses
	virtual void keyReleased(const sf::Keyboard::Key& key) = 0;					// Handles key releases
	virtual void mousePressed(const sf::Event::MouseButtonEvent& mouse) = 0;	// Handles mouse presses
	virtual void mouseReleased(const sf::Event::MouseButtonEvent& mouse) = 0;	// Handles mouse releases
	virtual void mouseMoved(const sf::Event::MouseMoveEvent& mouse) = 0;        // Handles mouse moves
};