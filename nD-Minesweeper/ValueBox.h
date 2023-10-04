#pragma once

#include "UIElementBase.h"
#include <functional>

// Valuebox for inputting numbers
class ValueBox : public UIElementBase
{
private:
	int value;							// The value
	std::string valueString;			// The string version of 'value'
	int min;							// Min boundary
	int max;							// Max boundary

public:
	bool selected = false;				// Bool which indicates if the valuebox is selected
	std::function<void()> onSelect;		// Function pointer pointing to code to execute on select
	std::function<void()> onDeselect;	// Function pointer pointing to code to execute on deselect


	ValueBox();
	ValueBox(const float x, const float y, const float width, const float height, const int value, const int min, const int max, const sf::Font& font, const unsigned int characterSize);
	int getValue() const;
	void setValue(const int value);
	int getMin() const;
	void setMin(const int min);
	int getMax() const;
	void setMax(const int max);
	void addDigit(const int digit);
	void addDigit(const sf::Keyboard::Key key);
	void removeDigit();
	void select();
	void deselect();
};

