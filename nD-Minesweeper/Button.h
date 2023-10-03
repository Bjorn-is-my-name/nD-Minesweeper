#pragma once

#include "UIElementBase.h"
#include <functional>

class Button : public UIElementBase
{
public:
	std::function<void()> onClick;

	Button();
	Button(const float x, const float y, const float width, const float height, const std::string text, const sf::Font& font, const unsigned int characterSize);
	void setText(const std::string text);
};

