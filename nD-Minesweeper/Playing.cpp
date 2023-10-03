#include "Playing.h"
#include "SettingsMenu.h"
#include <string>
#include <cstdlib>
#include <ctime>

/**
 * Starts the game
 * 
 * @param playSettings settings with which to play
 */
Playing::Playing(PlaySettings playSettings)
    : playSettings(playSettings)
{
    // Load files, exit game if unsuccesfull
    if (!flagTexture.loadFromFile("Flag.png"))
        Game::setState(nullptr);
    if (!textFont.loadFromFile("arial.ttf"))
        Game::setState(nullptr);

    // Create the bar that houses the controls
    controlsRect.setPosition(0, 0);
    controlsRect.setSize({ 1920, 90 });
    controlsRect.setFillColor(sf::Color::Black);
    controlsRect.setOutlineThickness(2);
    controlsRect.setOutlineColor(sf::Color::White);

    // Moving mode control
    movingModeLbl = Label(20, 20, "MovingMode (M): ", textFont, 30);
    labels.push_back(&movingModeLbl);
    
    movingModeBtn = Button(movingModeLbl.getX() + movingModeLbl.getWidth() + 20, movingModeLbl.getY() - movingModeLbl.getHeight() / 2, 50, 50, movingMode ? "On" : "Off", textFont, 30);
    movingModeBtn.onClick = [&]()
    {
        // Toggle moving mode 'On' or 'Off'
        movingMode = !movingMode;
        if (movingMode)
            lastMousePos = sf::Mouse::getPosition();

        movingModeBtn.setText(movingMode ? "On" : "Off");
    };
    buttons.push_back(&movingModeBtn);

    // Exit button to go back to pregame settings menu
    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = [&]() { Game::setState(new SettingsMenu(this->playSettings)); };
    buttons.push_back(&exitBtn);

    // Position of the board (visual)
    boardOffset = { 10, (int)(controlsRect.getSize().y + controlsRect.getOutlineThickness() + 10)};

    srand(time(0));

    int dimensions = playSettings.getDimensions();

    // Pre-allocate the memory for the vectors
    board.reserve(getMax(dimensions));
    tempTileSizes.resize(getMax(dimensions), std::vector<int>(dimensions));
    dimensionOffsets.resize(dimensions, 0);

    setOffsets();
    generateBoard(dimensions - 1, new std::vector<int>(dimensions, 0));

    // Add all the neighbors for each tile
    for (Tile& tile : board)
    {
        tile.addNeighbor(&tile);
        setNeighbors(tile, 0);
        tile.removeNeighbor(0);
    }
}

// Generate all the tiles on the board
void Playing::generateBoard(int dimension, std::vector<int>* counters)
{
    // Keep going in to a lower dimension, if at first dimension, create the tile
    for (int i = 0; i < playSettings.getDimensionSize(dimension); i++)
    {
        if (dimension > 0)
        {
            generateBoard(dimension - 1, counters);
            // Reset the counter if generating the next(higher) dimension started
            counters->at(dimension - 1) = 0;
        }
        else
            createTile(counters);

        // Update the counter for current dimension
        counters->at(dimension)++;
    }
}

// Create the tile based of the counters
void Playing::createTile(std::vector<int>* counters)
{
    // Startposition is the position in first and second dimension
    int x = counters->at(0);
    int y = 0;

    // Drawing offset (space added to have the cells be grouped by dimension), starts at the passed cells in the first and second dimension
    int xOffset = counters->at(0) * playSettings.getExtraSpace(0);
    int yOffset = 0;

    // Only set y and yOffset when there are more dimensions then 1
    if (playSettings.getDimensions() > 1)
    {
        y = counters->at(1);
        yOffset = counters->at(1) * playSettings.getExtraSpace(1);
    }

    // Add the maximum number of tiles leading up to the current tile for each dimension
    for (int i = 2; i < counters->size(); i++)
    {
        // For uneven dimensions, add to y
        if (i % 2)
        {
            y += counters->at(i) * getMaxExtended(i, 1, 2);
            // Add the space from all passed uneven dimensions to y offset
            for (int j = 0; j < counters->at(i); j++)
                yOffset += dimensionOffsets.at(i);
        }
        // For even dimensions, add to x
        else
        {
            x += counters->at(i) * getMaxExtended(i, 0, 2);
            // Add the space from all passed even dimensions to x offset
            for (int j = 0; j < counters->at(i); j++)
                xOffset += dimensionOffsets.at(i);
        }
    }

    // Create the tile and add it to the board
    board.push_back(Tile(board.size(), x, y, xOffset, yOffset, playSettings.tileSize));
    // Store the counters for this tile to be used when 
    tempTileSizes.at(board.size() - 1) = std::vector<int>(*counters);
}

// Set all the neighbors of a tile
void Playing::setNeighbors(Tile& tile, int dimension)
{
    // Go from lowest dimension to highest
    if (dimension < playSettings.getDimensions())
    {
        // For every neighbor found, check for their neighbor in the same dimension
        for (Tile *neighbor : tile.getNeighbors())
        {
            // Get the max number of tiles in current dimension
            int maxTiles = getMax(dimension);
            // Get the left or up neighbor its index by subtracting the number of tiles
            if (neighbor->getIndex() - maxTiles >= 0)
            {
                Tile* extraNeighbor = &board.at(neighbor->getIndex() - maxTiles);
                // If the neighbor is allowed, add it
                if (neighborAllowed(tempTileSizes.at(tile.getIndex()), tempTileSizes.at(extraNeighbor->getIndex())))
                    tile.addNeighbor(extraNeighbor);
            }
            // Get the right or down neighbor its index by adding the number of tiles
            if (neighbor->getIndex() + maxTiles < board.size())
            {
                Tile* extraNeighbor = &board.at(neighbor->getIndex() + maxTiles);
                // If the neighbor is allowed, add it
                if (neighborAllowed(tempTileSizes.at(tile.getIndex()), tempTileSizes.at(extraNeighbor->getIndex())))
                    tile.addNeighbor(extraNeighbor);
            }
        }
        // Go to the next dimension
        setNeighbors(tile, dimension + 1);
    }
}

// Check if a neighbor is allowed by checking the difference in position (done by subtracting the counters)
bool Playing::neighborAllowed(std::vector<int> cellSizes, std::vector<int> neighborSizes)
{
    // The neighbors are cut off at the sides of the first and second dimension, the neighbors that have to be cut off are more then 1 (row, colum, matrix, etc..) away
    for (int i = 0; i < cellSizes.size(); i++)
    {
        int difference = cellSizes.at(i) - neighborSizes.at(i);
        if (difference > 1 || difference < -1)
            return false;
    }
    return true;
}

// Set the drawing offsets for each dimension
void Playing::setOffsets()
{
    for (int i = 0; i < playSettings.getDimensions(); i++)
        dimensionOffsets.at(i) = playSettings.getExtraSpace(i) + calculateOffset(i, true);
}

// Calculate the offset by adding the offsets of already passed dimensions
int Playing::calculateOffset(int dimension, bool firstCall)
{
    int total = 0;

    // If its not the first call of the dimension add the extra space
    if (!firstCall)
        total += playSettings.getExtraSpace(dimension);

    // If there are still lower dimensions, make the first call to that dimension and create the pattern of the lower dimension
    if (dimension > 1)
    {
        total += calculateOffset(dimension - 2, true);
        // Repeat the pattern for the amount of times the dimension is in the higher dimension - 1
        for (int i = 0; i < playSettings.getDimensionSize(dimension - 2) - 1; i++)
            total += calculateOffset(dimension - 2);
    }

    return total;
}

// Get the maximum number of tiles in dimension
int Playing::getMax(int dimension)
{
    int total = 1;
    for (int i = 0; i < dimension; i++)
        total *= playSettings.getDimensionSize(i);
    return total;
}

// Get the maximum number of tiles in dimension (only counting specific dimensions)
int Playing::getMaxExtended(int dimension, int start, int increment)
{
    int total = 1;
    for (int i = start; i < dimension; i += increment)
        total *= playSettings.getDimensionSize(i);
    return total;
}

// Update the game
void Playing::update()
{
    sf::Vector2i currentMousePos = sf::Mouse::getPosition();

    // If moving mode is enabled and right click is down, make the board follow the mouse
    if (movingMode && sf::Mouse::isButtonPressed(sf::Mouse::Right))
        boardOffset -= lastMousePos - currentMousePos;

    // Update the last mouse position (position on previous frame)
    lastMousePos = currentMousePos;
}

// Draw everything on the screen
void Playing::draw(sf::RenderWindow& window)
{
    sf::Sprite flagSprite;
    flagSprite.setTexture(flagTexture);
    flagSprite.setScale(playSettings.tileSize / flagSprite.getLocalBounds().width, playSettings.tileSize / flagSprite.getLocalBounds().height);

    sf::Text valueText;
    valueText.setFont(textFont);
    valueText.setCharacterSize(playSettings.tileSize);
    valueText.setFillColor(sf::Color::Red);
    valueText.setOrigin(-(playSettings.tileSize / 4.0f), playSettings.tileSize / 8.0f);

    // Draw all tiles
    for (Tile& tile : board)
    {
        sf::RectangleShape rect = tile.getDrawingRect();
        rect.move(boardOffset.x, boardOffset.y);

        sf::Vector2f rectPos = rect.getPosition();
        if (rectPos.x + playSettings.tileSize < 0 || rectPos.x >= 1920 || rectPos.y + playSettings.tileSize < controlsRect.getSize().y || rectPos.y >= 1080)
            continue;

        window.draw(rect);

        if (tile.isFlagged())
        {
            flagSprite.setPosition(rectPos);
            window.draw(flagSprite);
        }

        if (tile.isVisible() && tile.getValue() > 0)
        {
            valueText.setString(std::to_string(tile.getValue()));
            valueText.setPosition(rectPos);
            window.draw(valueText);
        }
    }

    drawNeighbors(window);

    window.draw(controlsRect);

    for (auto& label : labels)
        label->draw(window);

    for (auto& button : buttons)
        button->draw(window);
}

void Playing::drawNeighbors(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    mousePos -= boardOffset;

    for (Tile& tile : board)
    {
        sf::Vector2f rectPos = tile.getDrawingRect().getPosition();
        if (rectPos.x + boardOffset.x + playSettings.tileSize < 0 || rectPos.x + boardOffset.x >= 1920 || rectPos.y + boardOffset.y + playSettings.tileSize < 0 || rectPos.y + boardOffset.y >= 1080)
            continue;

        if (tile.pointOnRect(mousePos.x, mousePos.y))
        {
            sf::RectangleShape rect = tile.getDrawingRect();
            rect.move((float)boardOffset.x, (float)boardOffset.y);
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Cyan);
            window.draw(rect);

            for (Tile* neighbor : tile.getNeighbors())
            {
                sf::Vector2f rectPos = neighbor->getDrawingRect().getPosition();
                if (rectPos.x + boardOffset.x + playSettings.tileSize < 0 || rectPos.x + boardOffset.x >= 1920 || rectPos.y + boardOffset.y + playSettings.tileSize < 0 || rectPos.y + boardOffset.y >= 1080)
                    continue;

                rect.setPosition(rectPos);
                rect.move((float)boardOffset.x, (float)boardOffset.y);
                window.draw(rect);
            }
            return;
        }
    }
}

void Playing::keyPressed(const sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Escape:
        Game::setState(new SettingsMenu(playSettings));
        break;
    case sf::Keyboard::M:
        movingMode = !movingMode;
        if (movingMode)
            lastMousePos = sf::Mouse::getPosition();

        movingModeBtn.setText(movingMode ? "On" : "Off");
        break;
    default:
        break;
    }
}

void Playing::mousePressed(const sf::Event::MouseButtonEvent event)
{
    
}

void Playing::mouseReleased(const sf::Event::MouseButtonEvent event)
{
    for (auto& button : buttons)
    {
        if (button->pointOnRect(event.x, event.y))
        {
            button->onClick();
            return;
        }
    }

    switch (event.button)
    {
    case sf::Mouse::Left:
        if (movingMode)
            return;

        for (Tile& tile : board)
        {
            if (tile.pointOnRect(event.x - boardOffset.x, event.y - boardOffset.y))
            {
                if (!firstTileClicked)
                {
                    generateBombs(tile);
                    firstTileClicked = true;
                }
                else if (tile.isBomb())
                {
                    lose();
                    return;
                }

                tile.setVisible(true);
                tile.setFlagged(false);
                tile.setColor(sf::Color::White);

                if (tile.getValue() == 0)
                    for (Tile* neighbor : tile.getNeighbors())
                        revealTiles(*neighbor);
                return;
            }
        }
        break;
    case sf::Mouse::Right:
        if (movingMode)
            return;

        for (Tile& tile : board)
        {
            if (tile.pointOnRect(event.x - boardOffset.x, event.y - boardOffset.y) && !tile.isVisible())
            {
                tile.setFlagged(!tile.isFlagged());
                if (tile.isBomb())
                {
                    if (tile.isFlagged())
                        bombsFlagged++;
                    else
                        bombsFlagged--;

                    if (bombsFlagged == playSettings.getBombs())
                        win();
                }
                return;
            }
        }
        break;
    default:
        break;
    }
}

void Playing::revealTiles(Tile& tile)
{
    if (tile.isVisible())
        return;

    tile.setVisible(true);
    tile.setFlagged(false);
    tile.setColor(sf::Color::White);

    if (tile.getValue() == 0)
        for (Tile* neighbor : tile.getNeighbors())
            if (!neighbor->isVisible())
                revealTiles(*neighbor);
}

void Playing::generateBombs(Tile& tile)
{
    if (playSettings.getBombs() == playSettings.getMax())
    {
        lose();
        return;
    }

    std::vector<int> possibleIndexes(board.size());

    for (int i = 0; i < board.size(); i++)
        possibleIndexes.at(i) = i;
    possibleIndexes.erase(possibleIndexes.begin() + tile.getIndex());

    for (int i = 0; i < playSettings.getBombs(); i++)
    {
        int index = rand() % possibleIndexes.size();
        board.at(possibleIndexes.at(index)).setBomb(true);
        possibleIndexes.erase(possibleIndexes.begin() + index);
    }

    for (Tile& tile : board)
    {
        int value = 0;
        for (Tile* neighbor : tile.getNeighbors())
            if (neighbor->isBomb())
                value++;
        tile.setValue(value);
    }
}

void Playing::win()
{
    Game::setState(new SettingsMenu(playSettings));
}

void Playing::lose()
{
    Game::setState(new SettingsMenu(playSettings));
}