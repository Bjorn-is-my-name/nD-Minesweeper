#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Tile
{
private:
	int index;
	sf::RectangleShape drawingRect;
	int x;
	int y;
	std::vector<Tile*> neighbors;
	int value;
	bool bomb = false;
	bool visible = false;
	bool flagged = false;

public:
	Tile(int, int, int, int, int, int);
	bool pointOnRect(int, int);

	#pragma region Getters
	int getIndex();
	sf::RectangleShape getDrawingRect();
	int getX();
	int getY();
	std::vector<Tile*> getNeighbors();
	int getValue();
	bool isBomb();
	bool isVisible();
	bool isFlagged();
	#pragma endregion
	#pragma region Setters
	void addNeighbor(Tile*);
	void removeNeighbor(int);
	void setValue(int);
	void setVisible(const bool);
	void setFlagged(const bool);
	void setColor(sf::Color);
	void setBomb(bool);
	#pragma endregion
};

