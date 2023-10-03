#include "Button.h"

Button::Button()
	: UIElementBase()
{
	onClick = []() {};
}

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
	text.setString(content);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
}

void Button::setText(const std::string content)
{
	text.setString(content);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
}