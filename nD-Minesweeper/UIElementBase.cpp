#include "UIElementBase.h"

/**
 * Initializes the UI element
 */
UIElementBase::UIElementBase()
{

}

/**
 * Draws the UI element
 * 
 * @param window the window to draw on
 */
void UIElementBase::draw(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(text);
}

/**
 * Checks if the point collides with the rectangle
 * 
 * @param x2, y2 position to check for collision
 * @return bool 'true' if collision is detected else 'false'
 */
bool UIElementBase::pointOnRect(const float x2, const float y2) const
{
	const float x = getX();
	const float y = getY();
	const float w = getWidth();
	const float h = getHeight();

	return x2 >= x && x2 < x + w && y2 >= y && y2 < y + h;
}

float UIElementBase::getX() const
{
	return rect.getPosition().x;
}

float UIElementBase::getY() const
{
	return rect.getPosition().y;
}

float UIElementBase::getWidth() const
{
	return rect.getSize().x;
}

float UIElementBase::getHeight() const
{
	return rect.getSize().y;
}