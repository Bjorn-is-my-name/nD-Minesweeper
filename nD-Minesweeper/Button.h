#pragma once

#include "UIElementBase.h"
#include <functional>

// Button class for creating buttons
class Button : public UIElementBase
{
public:
	std::function<void()> onClick;	// Function pointer pointing to code to execute on click

	Button();
	Button(const float x, const float y, const float width, const float height, const std::string text, const sf::Font& font, const unsigned int characterSize);
	void setText(const std::string text);
};

