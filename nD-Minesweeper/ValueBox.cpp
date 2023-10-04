#include "ValueBox.h"
#include "Game.h"

/**
 * Creates the object
 */
ValueBox::ValueBox()
	: UIElementBase()
{
	onSelect = []() {};
	onDeselect = []() {};
}

/**
 * Creates the object
 * 
 * @param x, y the position of the valuebox
 * @param width, height the size of the valuebox
 * @param value default value of the valuebox
 * @param minValue, maxValue the value boundaries
 * @param font the text font
 * @param characterSize the size of the text
 */
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

/**
 * Returns the value of the valuebox
 * 
 * @return 'value'
 */
int ValueBox::getValue() const
{
	return value;
}

/**
 * Sets the value of the valuebox
 * 
 * @param newValue the new value
 */
void ValueBox::setValue(const int newValue)
{
	// Check if its within the bounds
	if (newValue < min || newValue > max)
		return;

	// Update the value and text
	value = newValue;
	valueString = std::to_string(value);
	text.setString(valueString);
	
	// Re-center the text
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
}

/**
 * Returns the maximum value boundary of the valuebox
 *
 * @return 'max'
 */
int ValueBox::getMax() const
{
	return max;
}

/**
 * Sets the max value boundary of the valuebox
 * 
 * If the new max is lower then the current value, update the value to 'max'
 *
 * @param newMax the new max
 */
void ValueBox::setMax(const int newMax)
{
	max = newMax;

	if (max < value)
		setValue(max);
}

/**
 * Returns the minimum value boundary of the valuebox
 *
 * @return 'min'
 */
int ValueBox::getMin() const
{
	return min;
}

/**
 * Sets the min value boundary of the valuebox
 *
 * If the new min is higher then the current value, update the value to 'min'
 *
 * @param newMin the new min
 */
void ValueBox::setMin(const int newMin)
{
	min = newMin;

	if (min > value)
		setValue(min);
}

/**
 * Adds a digit to the valuebox
 *
 * @param digit the digit to add
 */
void ValueBox::addDigit(const int digit)
{
	setValue(value * 10 + digit);
}

/**
 * Adds a digit to the valuebox based of a pressed key
 *
 * @param key the pressed key
 */
void ValueBox::addDigit(const sf::Keyboard::Key key)
{
	// Check number keys and numpad number keys
	if (key >= 26 && key <= 35)
		addDigit(key - 26);
	else if (key >= 75 && key <= 84)
		addDigit(key - 75);
}

/**
 * Removes a digit from the valuebox
 */
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

/**
 * Selects the valuebox
 * 
 * Updates the 'selected' status, highlights the valuebox and executes 'onSelect'
 */
void ValueBox::select()
{
	selected = true;
	rect.setOutlineColor(sf::Color::Cyan);
	onSelect();
}

/**
 * Deselects the valuebox
 * 
 * Updates the 'selected' status, removes the highlight from the valuebox and executes 'onDeselect'
 */
void ValueBox::deselect()
{
	selected = false;
	rect.setOutlineColor(sf::Color::White);
	
	// If the valuebox is left empty, reset it to 'min'
	if (valueString == "")
		setValue(min);
	
	onDeselect();
}