#pragma once

#include <SFML/Graphics.hpp>

class UIElementBase
{
protected:
	sf::Text text;
	sf::RectangleShape rect;

public:
	UIElementBase();
	virtual void draw(sf::RenderWindow& window);
	virtual bool pointOnRect(const float x, const float y) const;
	virtual float getX() const;
	virtual float getY() const;
	virtual float getWidth() const;
	virtual float getHeight() const;
};

