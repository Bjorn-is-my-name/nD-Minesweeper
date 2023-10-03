#include "Tooltip.h"

ToolTip::ToolTip()
	: UIElementBase()
{

}

ToolTip::ToolTip(const float x, const float y, const float width, const float height, const std::string content, const sf::Font& font, const unsigned int characterSize)
{
	rect.setPosition(x, y);
	rect.setSize({ width, height });

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

	tooltipRect.setPosition(x + width, y);
	tooltipRect.setFillColor(sf::Color::Black);
	tooltipRect.setOutlineThickness(1);
	tooltipRect.setOutlineColor(sf::Color::White);

	sf::Vector2f tooltipRectPos = tooltipRect.getPosition();
	tooltip.setPosition(tooltipRectPos.x, tooltipRectPos.y + height / 2);
	tooltip.setCharacterSize(characterSize);
	tooltip.setFont(font);
	tooltip.setString(content);
	
	sf::FloatRect tooltipBounds = tooltip.getLocalBounds();
	sf::Vector2f padding = { 20, 20 };
	tooltipRect.setSize({ tooltipBounds.width + padding.x, tooltipBounds.height + padding.y });
	tooltip.setOrigin(tooltipBounds.left - padding.x / 2, tooltipBounds.top + tooltipBounds.height / 2 - padding.y / 2);

	float h = tooltipRect.getSize().y;
	tooltipRect.move({ 10, -h - 10 });
	tooltip.move({ 10, -h - 10 });
}

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

void ToolTip::show()
{
	showTooltip = true;
}

void ToolTip::hide()
{
	showTooltip = false;
}