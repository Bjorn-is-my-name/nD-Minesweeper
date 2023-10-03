#include "ValueBox.h"
#include "Game.h"

ValueBox::ValueBox()
	: UIElementBase()
{
	onSelect = []() {};
	onDeselect = []() {};
}

ValueBox::ValueBox(const float x, const float y, const float width, const float height, const int value, const int minValue, const int maxValue, const sf::Font& font, const unsigned int characterSize)
	: ValueBox()
{
	rect.setPosition(x, y);
	rect.setSize({width, height});
	rect.setFillColor(sf::Color::Black);
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color::White);

	text.setPosition(x + width / 2, y + height / 2);
	text.setCharacterSize(characterSize);
	text.setFont(font);

	min = minValue;
	max = maxValue;
	setValue(value);
}

int ValueBox::getValue() const
{
	return value;
}

void ValueBox::setValue(const int newValue)
{
	if (newValue < min || newValue > max)
		return;

	value = newValue;
	valueString = std::to_string(value);
	text.setString(valueString);
	
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
}

int ValueBox::getMax() const
{
	return max;
}

void ValueBox::setMax(const int newMax)
{
	max = newMax;

	if (max < value)
		setValue(max);
}

int ValueBox::getMin() const
{
	return min;
}

void ValueBox::setMin(const int newMin)
{
	min = newMin;

	if (min > value)
		setValue(min);
}

void ValueBox::addDigit(const int digit)
{
	setValue(value * 10 + digit);
}

void ValueBox::addDigit(const sf::Keyboard::Key key)
{
	if (key >= 26 && key <= 35)
		addDigit(key - 26);
	else if (key >= 75 && key <= 84)
		addDigit(key - 75);
}

void ValueBox::removeDigit()
{
	int newValue = value / 10;
	if (newValue >= min)
		setValue(newValue);
	else
	{
		value = 0;
		text.setString("0");
	}
}

void ValueBox::select()
{
	selected = true;
	rect.setOutlineColor(sf::Color::Cyan);
	onSelect();
}

void ValueBox::deselect()
{
	selected = false;
	rect.setOutlineColor(sf::Color::White);
	
	if (valueString == "")
		setValue(min);
	
	onDeselect();
}