#include "PlaySettings.h"

/**
 * Initializes game settings with default settings
 */
PlaySettings::PlaySettings()
{
	tileSize = 30;
	dimensions = 4;
	dimensionSizes = { 5, 5, 5, 5 };
	extraSpacePerDimension = { 0, 0, 1, 1 };
	bombs = 25;
}

/**
 * Returns dimensions setting
 * 
 * @return 'dimensions'
 */
int PlaySettings::getDimensions() const
{
	return dimensions;
}

/**
 * Sets the dimensions setting
 *
 * @param newDimensions new value for 'dimensions' setting
 */
void PlaySettings::setDimensions(int newDimensions)
{
	// Add new dimension sizes with default value if the new dimension is higher, else delete redundant dimension sizes
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

/**
 * Returns dimension size of given dimension
 *
 * @param dimension dimension of which to get the size
 * @return size of dimension
 */
int PlaySettings::getDimensionSize(int dimension) const
{
	if (dimension >= dimensionSizes.size())
		return 0;

	return dimensionSizes.at(dimension);
}

/**
 * Sets a dimension size
 *
 * @param dimension dimension of which to set the size of
 * @param size the new size of the dimension
 */
void PlaySettings::setDimensionSize(int dimension, int size)
{
	dimensionSizes[dimension] = size;
}

/**
 * Returns extra space of given dimension
 *
 * @param dimension dimension of which to get the extra space
 * @return extra space of dimension
 */
int PlaySettings::getExtraSpace(int dimension) const
{
	if (dimension >= extraSpacePerDimension.size())
		return 0;

	return extraSpacePerDimension.at(dimension);
}

/**
 * Sets a dimension its extra space
 *
 * @param dimension dimension of which to set the extra space of
 * @param extraSpace the new extra space of the dimension
 */
void PlaySettings::setExtraSpace(int dimension, int extraSpace)
{
	extraSpacePerDimension[dimension] = extraSpace;
}

/**
 * Returns the number of bombs
 *
 * @return 'bombs'
 */
int PlaySettings::getBombs() const
{
	return bombs;
}

/**
 * Sets the amount of bombs the play with
 *
 * @param newBombs the new amount of bombs
 */
void PlaySettings::setBombs(int newBombs)
{
	bombs = newBombs;
}

/**
 * Returns the number of tiles that fit in the game
 *
 * @return max number of tiles
 */
int PlaySettings::getMax() const
{
	int total = 1;
	for (int i = 0; i < dimensions; i++)
		total *= dimensionSizes.at(i);
	return total;
}