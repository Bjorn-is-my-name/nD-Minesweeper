#include "PlaySettings.h"

PlaySettings::PlaySettings()
{
	cellSize = 30;
	dimensions = 4;
	dimensionSizes = { 5, 5, 5, 5 };
	extraSpacePerDimension = { 0, 0, 1, 1 };
	bombs = 25;
}

int PlaySettings::getDimensions() const
{
	return dimensions;
}

void PlaySettings::setDimensions(int newDimensions)
{
	if (newDimensions > dimensions)
	{
		for (int i = 0; i < newDimensions - dimensions; i++)
		{
			dimensionSizes.push_back(5);
			extraSpacePerDimension.push_back(extraSpacePerDimension.size() / 2);
		}
	}
	else
	{
		for (int i = 0; i < dimensions - newDimensions; i++)
		{
			dimensionSizes.pop_back();
			extraSpacePerDimension.pop_back();
		}
	}

	dimensions = newDimensions;
}

int PlaySettings::getDimensionSize(int dimension) const
{
	if (dimension >= dimensionSizes.size())
		return 0;

	return dimensionSizes.at(dimension);
}

void PlaySettings::setDimensionSize(int dimension, int size)
{
	dimensionSizes[dimension] = size;
}

int PlaySettings::getExtraSpace(int dimension) const
{
	if (dimension >= extraSpacePerDimension.size())
		return 0;

	return extraSpacePerDimension.at(dimension);
}

void PlaySettings::setExtraSpace(int dimension, int extraSpace)
{
	extraSpacePerDimension[dimension] = extraSpace;
}

int PlaySettings::getBombs() const
{
	return bombs;
}

void PlaySettings::setBombs(int newBombs)
{
	bombs = newBombs;
}

int PlaySettings::getMax() const
{
	int total = 1;
	for (int i = 0; i < dimensions; i++)
		total *= dimensionSizes.at(i);
	return total;
}