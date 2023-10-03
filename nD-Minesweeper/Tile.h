#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Tile
{
private:
	int index;								// Index of the tile, used for identification
	sf::RectangleShape drawingRect;			// Rectangle to draw
	int x;									// X position of the tile
	int y;									// Y position of the tile
	std::vector<Tile*> neighbors;			// List of neighboring tiles
	int value;								// Value of the tile
	bool bomb = false;						// Bool if the tile is a bomb
	bool visible = false;					// Bool if the tile is revealed
	bool flagged = false;					// Bool if the tile is flagged

public:
	Tile(int, int, int, int, int, int);
	bool pointOnRect(int, int);				// Function which checks for collision with the tile

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

