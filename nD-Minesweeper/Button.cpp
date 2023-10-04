#include "Button.h"

/**
 * Creates the button
 */
Button::Button()
	: UIElementBase()
{
	onClick = []() {};
}

/**
 * Creates the button
 * 
 * @param x, y the position of the button
 * @param width, heigth the size of the button
 * @param content the text in the button
 * @param font the text font
 * @param characterSize the text size
 */
Button::Button(const float x, const float y, const float width, const float height, const std::string content, const sf::Font& font, const unsigned int characterSize)
	: Button()
{
	rect.setPosition(x, y);
	rect.setSize({width, height});
	rect.setFillColor(sf::Color::Black);
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color::White);

	text.setPosition(x + width / 2, y + height / 2);
	text.setCharacterSize(characterSize);
	text.setFont(font);
	setText(content);
}

/**
 * Sets the text in the button
 * 
 * @param content the text
 */
void Button::setText(const std::string content)
{
	text.setString(content);

	// Center the text in the button
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
}