#pragma once

#include "UIElementBase.h"

class ToolTip : public UIElementBase
{
private:
	sf::CircleShape circle;
	sf::RectangleShape tooltipRect;
	sf::Text tooltip;
	bool showTooltip;

public:
	ToolTip();
	ToolTip(const float x, const float y, const float width, const float height, const std::string content, const sf::Font& font, const unsigned int characterSize);
	void draw(sf::RenderWindow& window) override;
	void show();
	void hide();
};

