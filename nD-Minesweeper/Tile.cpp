#include "Tile.h"

Tile::Tile(int index, int x, int y, int xOffset, int yOffset, int cellSize)
{
	this->index = index;
	this->x = x;
	this->y = y;
	this->value = 0;

	drawingRect.setPosition((x + xOffset) * cellSize, (y + yOffset) * cellSize);
	drawingRect.setSize(sf::Vector2f(cellSize, cellSize));
	drawingRect.setFillColor(sf::Color(128, 128, 128));
	drawingRect.setOutlineThickness(-2);
	drawingRect.setOutlineColor(sf::Color::Black);
}
#include <iostream>
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