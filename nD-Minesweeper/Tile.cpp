#include "Tile.h"

/**
 * Creates the tile
 * 
 * @param index the index of the tile
 * @param x, y position of the tile
 * @param xOffset, yOffset visual offset for the tile
 * @param tileSize size of the tile
 */
Tile::Tile(int index, int x, int y, int xOffset, int yOffset, int tileSize)
{
	this->index = index;
	this->x = x;
	this->y = y;
	this->value = 0;

	drawingRect.setPosition((x + xOffset) * tileSize, (y + yOffset) * tileSize);
	drawingRect.setSize(sf::Vector2f(tileSize, tileSize));
	drawingRect.setFillColor(sf::Color(128, 128, 128));
	drawingRect.setOutlineThickness(-2);
	drawingRect.setOutlineColor(sf::Color::Black);
}

/**
 * Checks for collision with the tile
 * 
 * @param x, y position to check for collision
 * @return collision 'true' or 'false'
 */
bool Tile::pointOnRect(int x, int y)
{
	sf::Vector2f pos = drawingRect.getPosition();
	sf::Vector2f size = drawingRect.getSize();
	return x >= pos.x && x <= pos.x + size.x && y >= pos.y && y <= pos.y + size.y;
}

#pragma region Getters
int Tile::getIndex()
{
	return index;
}

sf::RectangleShape Tile::getDrawingRect()
{
	return drawingRect;
}

int Tile::getX()
{
	return x;
}

int Tile::getY()
{
	return y;
}

std::vector<Tile*> Tile::getNeighbors()
{
	return neighbors;
}

int Tile::getValue()
{
	return value;
}

bool Tile::isBomb()
{
	return bomb;
}

bool Tile::isVisible()
{
	return visible;
}

bool Tile::isFlagged()
{
	return flagged;
}
#pragma endregion
#pragma region Setters
void Tile::addNeighbor(Tile* neighbor)
{
	neighbors.push_back(neighbor);
}

void Tile::removeNeighbor(int index)
{
	neighbors.erase(neighbors.begin() + index);
}

void Tile::setValue(int value)
{
	this->value = value;
}

void Tile::setVisible(const bool visible)
{
	this->visible = visible;
}

void Tile::setFlagged(const bool flagged)
{
	this->flagged = flagged;
}

void Tile::setColor(sf::Color color)
{
	drawingRect.setFillColor(color);
}

void Tile::setBomb(bool bomb)
{
	this->bomb = bomb;
}
#pragma endregion