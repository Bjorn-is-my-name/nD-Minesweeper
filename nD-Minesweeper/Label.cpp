#include "Label.h"

Label::Label()
	: UIElementBase()
{

}

Label::Label(const float x, const float y, const std::string content, const sf::Font& font, const unsigned int characterSize)
	: Label()
{
	text.setPosition(x, y);
	text.setCharacterSize(characterSize);
	text.setFont(font);
	text.setString(content);
}

void Label::setText(const std::string content)
{
	text.setString(content);
}

void Label::draw(sf::RenderWindow& window)
{
	window.draw(text);
}

bool Label::pointOnRect(const float x, const float y) const
{
	return false;
}

float Label::getX() const
{
	return text.getPosition().x + text.getLocalBounds().left;
}

float Label::getY() const
{
	return text.getPosition().y + text.getLocalBounds().top;
}

float Label::getWidth() const
{
	return text.getLocalBounds().width;
}

float Label::getHeight() const
{
	return text.getLocalBounds().height;
}