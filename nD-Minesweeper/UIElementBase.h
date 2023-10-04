#pragma once

#include <SFML/Graphics.hpp>

// Base class for UI elements
class UIElementBase
{
protected:
	sf::Text text;				// Text object for drawing text
	sf::RectangleShape rect;	// RectangleShape for drawing a rectangle and/or acting as bounds

public:
	UIElementBase();
	virtual void draw(sf::RenderWindow& window);
	virtual bool pointOnRect(const float x, const float y) const;
	virtual float getX() const;
	virtual float getY() const;
	virtual float getWidth() const;
	virtual float getHeight() const;
};

