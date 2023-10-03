#pragma once

#include "UIElementBase.h"

class Label : public UIElementBase
{
public:
	Label();
	Label(const float x, const float y, const std::string content, const sf::Font& font, const unsigned int characterSize);
	void setText(const std::string content);
	void draw(sf::RenderWindow& window) override;
	bool pointOnRect(const float x, const float y) const override;
	float getX() const override;
	float getY() const override;
	float getWidth() const override;
	float getHeight() const override;
};

