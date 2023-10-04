#include "Label.h"

/**
 * Creates the label
 */
Label::Label()
	: UIElementBase()
{

}

/**
 * Creates the label
 * 
 * Only creates the text, not the rectangle
 * 
 * @param x, y the position of the label,
 * @param content the text of the label
 * @param font the text font
 * @param characterSize the size of the text
 */
Label::Label(const float x, const float y, const std::string content, const sf::Font& font, const unsigned int characterSize)
	: Label()
{
	text.setPosition(x, y);
	text.setCharacterSize(characterSize);
	text.setFont(font);
	text.setString(content);
}

/**
 * Sets the text
 * 
 * @param content the new text
 */
void Label::setText(const std::string content)
{
	text.setString(content);
}

/**
 * Draws the label
 * 
 * @param window the window to draw on
 */
void Label::draw(sf::RenderWindow& window)
{
	window.draw(text);
}

/**
 * Checks for collision
 * 
 * Collision is always false because the label has no hitbox
 * 
 * @return 'false'
 */
bool Label::pointOnRect(const float x, const float y) const
{
	return false;
}

/**
 * Returns the x coordinate of the text, not the managing rectangle (created by SFML)
 * 
 * @return x coordinate of text
 */
float Label::getX() const
{
	return text.getPosition().x + text.getLocalBounds().left;
}

/**
 * Returns the y coordinate of the text, not the managing rectangle (created by SFML)
 * 
 * @return y coordinate of text
 */
float Label::getY() const
{
	return text.getPosition().y + text.getLocalBounds().top;
}

/**
 * Returns the width of the text, not the managing rectangle (created by SFML)
 * 
 * @return width of the text
 */
float Label::getWidth() const
{
	return text.getLocalBounds().width;
}

/**
 * Returns the height of the text, not the managing rectangle (created by SFML)
 *
 * @return height of the text
 */
float Label::getHeight() const
{
	return text.getLocalBounds().height;
}