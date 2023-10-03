#pragma once

#include <vector>

class PlaySettings
{
private:
	int dimensions;
	std::vector<int> dimensionSizes;
	std::vector<int> extraSpacePerDimension;
	int bombs;

public:
	int cellSize;

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

