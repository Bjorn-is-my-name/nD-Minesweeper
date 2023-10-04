#pragma once

#include "UIElementBase.h"

// Tooltip class for creating tooltips
class ToolTip : public UIElementBase
{
private:
	sf::CircleShape circle;			// The tooltip circle
	sf::RectangleShape tooltipRect;	// The tooltip rectangle which holds the text
	sf::Text tooltip;				// The tooltip text
	bool showTooltip;				// Bool which indicates if the tooltip is shown or hidden

public:
	ToolTip();
	ToolTip(const float x, const float y, const float width, const float height, const std::string content, const sf::Font& font, const unsigned int characterSize);
	void draw(sf::RenderWindow& window) override;
	void show();
	void hide();
};

