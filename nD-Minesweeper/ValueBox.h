#pragma once

#include "UIElementBase.h"
#include <functional>

class ValueBox : public UIElementBase
{
private:
	int value;
	std::string valueString;
	int min;
	int max;

public:
	bool selected = false;
	std::function<void()> onSelect;
	std::function<void()> onDeselect;


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

