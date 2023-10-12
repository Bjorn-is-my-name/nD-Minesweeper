#pragma once

#include "GameState.h"
#include <vector>
#include "Tile.h"
#include "PlaySettings.h"
#include "Label.h"
#include "Button.h"

class Playing : public GameState
{
private:
	std::vector<Tile> board;						// Board to store all the tiles
	sf::Vector2i boardOffset;						// X and Y offset for drawing the tiles
	std::vector<std::vector<int>> tempTileCounters;	// Holds counters of cells for calculating neighbors - counter: stores how many times a dimension is passed, indicates tile position
	PlaySettings playSettings;						// Settings with which the game is played
	std::vector<int> dimensionOffsets;				// Stores the position offset for the tiles
	sf::Vector2i lastMousePos;						// Mouse position in previous frame

	sf::Font textFont;								// Font for all the displayed text
	sf::Texture flagTexture;						// Flag texture for flagging bombs
	sf::RectangleShape controlsRect;				// Bar at top of screen which houses the ingame controls

	Label movingModeLbl;							// Label for the moving mode control
	std::vector<Label*> labels;						// Vector which holds all the labels on the page

	Button movingModeBtn;							// Button for toggeling the moving mode control
	Button exitBtn;									// Exit button to go back to pregame menu
	std::vector<Button*> buttons;					// Vector which holds all the buttons on the page

	bool firstTileClicked = false;					// Bool for checking if the game has been initiated
	int bombsFlagged = 0;							// Number of bombs flagged correctly
	bool movingMode = false;						// Bool for the moving mode control

	void drawNeighbors(sf::RenderWindow& window);										// Highlights the neighbors of the tile being hovered over
	void generateBoard(int dimension, std::vector<int>* counters);						// Generates the board
	void createTile(std::vector<int>* counters);										// Creates the tiles
	void setOffsets();																	// Sets the drawing offsets
	int calculateOffset(int dimension, bool firstCall = false);							// Calculates the drawing offsets
	int getMax(int dimension);															// Calculates the number of tiles that fit in the given dimension
	int getMaxExtended(int dimension, int start, int increment);						// Calculates the number of tiles that fit in the given dimension with extra options
	void generateBombs(Tile& tile);														// Generates all the bombs
	void revealTiles(Tile& tile);														// Reveals the clicked tile (and its neighbors if no mines are around)
	void setNeighbors(Tile& tile, int dimension);										// Sets the neighbors for each tile
	bool neighborAllowed(std::vector<int> tileSizes, std::vector<int> neighborSizes);	// Checks if a neighbor is within dimension bounds
	void win();																			// Triggers when the game is won
	void lose();																		// Triggers when the game is lost

public:
	Playing(const PlaySettings& playSettings = PlaySettings());
	void update() override;
	void draw(sf::RenderWindow& window) override;
	void keyPressed(const sf::Keyboard::Key& key) override;
	void keyReleased(const sf::Keyboard::Key& key) override;
	void mousePressed(const sf::Event::MouseButtonEvent& mouse) override;
	void mouseReleased(const sf::Event::MouseButtonEvent& mouse) override;
	void mouseMoved(const sf::Event::MouseMoveEvent& mouse) override;
};