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
	std::vector<Tile> board;
	sf::Vector2i boardOffset;
	std::vector<std::vector<int>> tempTileSizes;
	PlaySettings playSettings;
	std::vector<int> dimensionOffsets;
	sf::Vector2i lastMousePos;

	sf::Font textFont;
	sf::Texture flagTexture;
	sf::RectangleShape controlsRect;

	Label movingModeLbl;
	std::vector<Label*> labels;

	Button movingModeBtn;
	Button exitBtn;
	std::vector<Button*> buttons;

	bool firstTileClicked = false;
	int bombsFlagged = 0;
	bool movingMode = false;

	void drawNeighbors(sf::RenderWindow& window);
	void generateBoard(int dimension, std::vector<int>* counters);
	void createTile(std::vector<int>* counters);
	void setOffsets();
	int calculateOffset(int dimension, bool firstCall = false);
	int getMax(int dimension);
	int getMaxExtended(int dimension, int start, int increment);
	void generateBombs(Tile& tile);
	void revealTiles(Tile& tile);
	void setNeighbors(Tile& tile, int dimension);
	bool neighborAllowed(std::vector<int> cellSizes, std::vector<int> neighborSizes);
	void win();
	void lose();

public:
	Playing(PlaySettings);
	void update() override;
	void draw(sf::RenderWindow&) override;
	void keyPressed(const sf::Keyboard::Key) override;
	void mousePressed(const sf::Event::MouseButtonEvent) override;
	void mouseReleased(const sf::Event::MouseButtonEvent) override;
};