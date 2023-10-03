#pragma once

#include <vector>

// Settings for the game
class PlaySettings
{
private:
	int dimensions;								// Dimension to play in
	std::vector<int> dimensionSizes;			// Sizes of each dimension
	std::vector<int> extraSpacePerDimension;	// Space between each dimension (visual)
	int bombs;									// Number of bombs

public:
	int tileSize;								// Size of the tiles

	PlaySettings();
	int getDimensions() const;
	void setDimensions(const int dimensions);
	int getDimensionSize(const int dimension) const;
	void setDimensionSize(const int dimension, const int size);
	int getExtraSpace(const int dimension) const;
	void setExtraSpace(const int dimension, const int space);
	int getBombs() const;
	void setBombs(int bombs);
	int getMax() const;
};

