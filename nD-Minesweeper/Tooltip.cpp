#include "Tooltip.h"

/**
 * Creates the tooltip
 */
ToolTip::ToolTip()
	: UIElementBase()
{
	showTooltip = false;
}

/**
 * Creates the tooltip
 * 
 * @param x, y the position of the tooltip
 * @param width, height the size of the tooltip
 * @param content the tooltip text
 * @param font the text font
 * @param characterSize the size of the text
 */
ToolTip::ToolTip(const float x, const float y, const float width, const float height, const std::string content, const sf::Font& font, const unsigned int characterSize)
	: ToolTip()
{
	// Create the rectangle for collision checks
	rect.setPosition(x, y);
	rect.setSize({ width, height });

	// Create a circle with a quistionmark to indicate a tooltip, center the text in the circle
	circle.setPosition(x, y);
	circle.setRadius(width / 2);
	circle.setFillColor(sf::Color::Black);
	circle.setOutlineThickness(1);
	circle.setOutlineColor(sf::Color::White);

	sf::Vector2f rectPos = rect.getPosition();
	text.setPosition(rectPos.x + width / 2, rectPos.y + height / 2);
	text.setCharacterSize(characterSize);
	text.setFont(font);
	text.setString("?");
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);

	// Create the rectangle for showing the tooltip
	tooltipRect.setPosition(x + width, y);
	tooltipRect.setFillColor(sf::Color::Black);
	tooltipRect.setOutlineThickness(1);
	tooltipRect.setOutlineColor(sf::Color::White);

	// Create the tooltip text
	sf::Vector2f tooltipRectPos = tooltipRect.getPosition();
	tooltip.setPosition(tooltipRectPos.x, tooltipRectPos.y + height / 2);
	tooltip.setCharacterSize(characterSize);
	tooltip.setFont(font);
	tooltip.setString(content);
	
	// Resize the tooltip rectangle based on the text size and center the text in the rectangle
	sf::FloatRect tooltipBounds = tooltip.getLocalBounds();
	sf::Vector2f padding = { 20, 20 };
	tooltipRect.setSize({ tooltipBounds.width + padding.x, tooltipBounds.height + padding.y });
	tooltip.setOrigin(tooltipBounds.left - padding.x / 2, tooltipBounds.top + tooltipBounds.height / 2 - padding.y / 2);

	// Move the tooltip rectangle and text slightly to top right of the tooltip indicator
	float h = tooltipRect.getSize().y;
	tooltipRect.move({ 10, -h - 10 });
	tooltip.move({ 10, -h - 10 });
}

/**
 * Draws the tooltip indicator
 * 
 * Only draws the tooltip if 'showTooltip' is 'true'
 */
void ToolTip::draw(sf::RenderWindow& window)
{
	window.draw(circle);
	window.draw(text);

	if (showTooltip)
	{
		window.draw(tooltipRect);
		window.draw(tooltip);
	}
}

/**
 * Sets the 'showTooltip' state to 'true'
 */
void ToolTip::show()
{
	showTooltip = true;
}

/**
 * Sets the 'showTooltip' state to 'false'
 */
void ToolTip::hide()
{
	showTooltip = false;
}