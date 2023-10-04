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

    // Pre-allocate the memory off the vectors
    board.reserve(getMax(dimensions));
    tempTileCounters.resize(getMax(dimensions), std::vector<int>(dimensions));
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

/**
 * Generate all the tiles on the board
 * 
 * @param dimension dimension to generate tiles in
 * @param counters dimension counters to keep track of positioning
 */
void Playing::generateBoard(int dimension, std::vector<int>* counters)
{
    // Keep going in to a lower dimension, if at first dimension, create the tile
    for (int i = 0; i < playSettings.getDimensionSize(dimension); i++)
    {
        if (dimension > 0)
        {
            generateBoard(dimension - 1, counters);
            // Reset the counter if generating the next (higher) dimension started
            counters->at(dimension - 1) = 0;
        }
        else
            createTile(counters);

        // Update the counter for current dimension
        counters->at(dimension)++;
    }
}

/**
 * Create the tile based of the counters
 * 
 * @param counters the number times each dimension is passed, used for calculating tile position
 */
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
    // Store the counters for this tile for finding its neighbors
    tempTileCounters.at(board.size() - 1) = std::vector<int>(*counters);
}

/**
 * Set all the neighbors of a tile
 * 
 * @param tile[in, out] the tile which its neighbors have to be set
 * @param dimension[in] the dimension to search in for neighbors
 */
void Playing::setNeighbors(Tile& tile, int dimension)
{
    // Go from lowest dimension to highest (recursively)
    if (dimension < playSettings.getDimensions())
    {
        // For every neighbor found, check for their neighbors in the same dimension
        for (Tile *neighbor : tile.getNeighbors())
        {
            int maxTiles = getMax(dimension);
            // Get the left or up neighbor its index by subtracting the number of tiles
            if (neighbor->getIndex() - maxTiles >= 0)
            {
                Tile* extraNeighbor = &board.at(neighbor->getIndex() - maxTiles);
                // If the neighbor is allowed, add it
                if (neighborAllowed(tempTileCounters.at(tile.getIndex()), tempTileCounters.at(extraNeighbor->getIndex())))
                    tile.addNeighbor(extraNeighbor);
            }
            // Get the right or down neighbor its index by adding the number of tiles
            if (neighbor->getIndex() + maxTiles < board.size())
            {
                Tile* extraNeighbor = &board.at(neighbor->getIndex() + maxTiles);
                // If the neighbor is allowed, add it
                if (neighborAllowed(tempTileCounters.at(tile.getIndex()), tempTileCounters.at(extraNeighbor->getIndex())))
                    tile.addNeighbor(extraNeighbor);
            }
        }
        // Go to the next dimension
        setNeighbors(tile, dimension + 1);
    }
}

/**
 * Check if a neighbor is allowed
 *
 * A neighbor is allowed if the difference in position (done by subtracting the counters) is at most 1 away
 * 
 * @param tileCounters, the counters that indicate the tile its position
 * @param neighborCounters, the counters that indicate the neighbor its position
 * @return bool 'true' or 'false' which indicates if a neighbor is allowed
 */
bool Playing::neighborAllowed(std::vector<int> tileCounters, std::vector<int> neighborCounters)
{
    // The neighbors are cut off at the sides of the first and second dimension, the neighbors that have to be cut off are more then 1 (row, colum, matrix, etc..) away
    for (int i = 0; i < tileCounters.size(); i++)
    {
        int difference = tileCounters.at(i) - neighborCounters.at(i);
        if (difference > 1 || difference < -1)
            return false;
    }
    return true;
}

/**
 * Set the drawing offsets for each dimension
 */
void Playing::setOffsets()
{
    for (int i = 0; i < playSettings.getDimensions(); i++)
        dimensionOffsets.at(i) = playSettings.getExtraSpace(i) + calculateOffset(i, true);
}

/**
 * Calculate the offset by adding the offsets of already passed dimensions
 * 
 * The function is called recursively to add up offsets of passed dimensions to get offset for current dimension
 * the bool 'firstCall' is necessary because the start of a dimension its pattern is slightly different from main pattern
 * 
 * @param dimension the dimension to calculate the offset for
 * @param firstCall indicates if this is the first call to the function for creating a dimension specific pattern
 * @return 'total' the total offset
 */
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

/**
 * Get the maximum number of tiles in the given dimension
 * 
 * @param dimension the dimension for which to calculate the number of tiles
 * @return 'total' the total number of tiles
 */
int Playing::getMax(int dimension)
{
    // Multiply all the dimension their sizes up to the input dimension
    int total = 1;
    for (int i = 0; i < dimension; i++)
        total *= playSettings.getDimensionSize(i);
    return total;
}

/**
 * Get the maximum number of tiles in dimension with extended paramters
 * 
 * @param dimension the dimension for which to calculate the number of tiles (stop)
 * @param start the starting dimension                                       (start)
 * @param increment how many dimensions to go higher                         (step)
 * @return 'total' the total number of tiles
 */
int Playing::getMaxExtended(int dimension, int start, int increment)
{
    int total = 1;
    for (int i = start; i < dimension; i += increment)
        total *= playSettings.getDimensionSize(i);
    return total;
}

/**
 * Update the game
 */
void Playing::update()
{
    sf::Vector2i currentMousePos = sf::Mouse::getPosition();

    // If moving mode is enabled and right click is down, make the board follow the mouse
    if (movingMode && sf::Mouse::isButtonPressed(sf::Mouse::Right))
        boardOffset -= lastMousePos - currentMousePos;

    lastMousePos = currentMousePos;
}

/**
 * Draw everything on the screen
 * 
 * @param window the window to draw on
 */
void Playing::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);

    // Create a sprite of the flag texture
    sf::Sprite flagSprite;
    flagSprite.setTexture(flagTexture);
    flagSprite.setScale(playSettings.tileSize / flagSprite.getLocalBounds().width, playSettings.tileSize / flagSprite.getLocalBounds().height);

    // Create a text object for drawing the number of nearby bombs
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

        // If the tile is outside the bounds, skip it
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

    // Draw the controls bar and its controls
    window.draw(controlsRect);

    for (auto& label : labels)
        label->draw(window);

    for (auto& button : buttons)
        button->draw(window);
}

/**
 * Highlights the neighbors of the cell being hovered over
 * 
 * @param window the window to draw on
 */
void Playing::drawNeighbors(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    mousePos -= boardOffset;

    // Check all tiles for collision with mouse position
    for (Tile& tile : board)
    {
        // Skip all the tiles which are outside the drawing bounds
        sf::Vector2f rectPos = tile.getDrawingRect().getPosition();
        if (rectPos.x + boardOffset.x + playSettings.tileSize < 0 || rectPos.x + boardOffset.x >= 1920 || rectPos.y + boardOffset.y + playSettings.tileSize < controlsRect.getSize().y || rectPos.y + boardOffset.y >= 1080)
            continue;

        // Do the collision check
        if (tile.pointOnRect(mousePos.x, mousePos.y))
        {
            // Highlight the hovered over tile
            sf::RectangleShape rect = tile.getDrawingRect();
            rect.move((float)boardOffset.x, (float)boardOffset.y);
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Cyan);
            window.draw(rect);

            // Get all the tile its neighbors and highlight them
            for (Tile* neighbor : tile.getNeighbors())
            {
                // Skip neighbors which are outside the drawing bounds
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

/**
 * Handles key presses
 *
 * @param key key that is pressed
 */
void Playing::keyPressed(const sf::Keyboard::Key key)
{
    switch (key)
    {
    // On 'Escape' go back to the pregame settings menu
    case sf::Keyboard::Escape:
        Game::setState(new SettingsMenu(playSettings));
        break;
    // On 'M' toggle moving mode on or off
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

/**
 * Handles mouse presses
 *
 * @param event mouse event which has pressed mousebutton and mouse position
 */
void Playing::mousePressed(const sf::Event::MouseButtonEvent event)
{
    
}

/**
 * Handles mouse releases
 *
 * @param event mouse event which has released mousebutton and mouse position
 */
void Playing::mouseReleased(const sf::Event::MouseButtonEvent event)
{
    // Check if a button is pressed
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
    // On 'Mouse.Left' reveal the tile if not in moving mode
    case sf::Mouse::Left:
        if (movingMode)
            return;

        // Find the tile being clicked
        for (Tile& tile : board)
        {
            if (tile.pointOnRect(event.x - boardOffset.x, event.y - boardOffset.y))
            {
                // On first click, generate the bombs
                if (!firstTileClicked)
                {
                    generateBombs(tile);
                    firstTileClicked = true;
                }
                // Check if the tile is a bomb
                else if (tile.isBomb())
                {
                    lose();
                    return;
                }

                // Reveal the tile
                tile.setVisible(true);
                tile.setFlagged(false);
                tile.setColor(sf::Color::White);

                // Reveal the tile its neighbors if no bombs are around
                if (tile.getValue() == 0)
                    for (Tile* neighbor : tile.getNeighbors())
                        revealTiles(*neighbor);
                return;
            }
        }
        break;
    // On 'Mouse.Right' flag or unflag the tile of not in moving mode
    case sf::Mouse::Right:
        if (movingMode)
            return;

        // Find the tile being clicked
        for (Tile& tile : board)
        {
            if (tile.pointOnRect(event.x - boardOffset.x, event.y - boardOffset.y) && !tile.isVisible())
            {
                // Change 'flagged' status to true or false based on old state
                tile.setFlagged(!tile.isFlagged());
                // If the tile is a bomb update the counter
                if (tile.isBomb())
                {
                    if (tile.isFlagged())
                        bombsFlagged++;
                    else
                        bombsFlagged--;

                    // If all the bombs are flagged, end the game
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

/**
 * Reveal tiles and their neighbors
 *
 * @param tile the tile to reveal and reveal neighbors of
 */
void Playing::revealTiles(Tile& tile)
{
    // Only reveal hidden tiles
    if (tile.isVisible())
        return;

    tile.setVisible(true);
    tile.setFlagged(false);
    tile.setColor(sf::Color::White);

    // Reveal hidden neighbors if no bombs are around
    if (tile.getValue() == 0)
        for (Tile* neighbor : tile.getNeighbors())
            if (!neighbor->isVisible())
                revealTiles(*neighbor);
}

/**
 * Generates all the bombs on the board
 *
 * @param tile the tile that got revealed first where no bombs may be placed
 */
void Playing::generateBombs(Tile& tile)
{
    // If the number of bombs is equal to the number of tiles its an instant loss (all tiles are bombs)
    if (playSettings.getBombs() == playSettings.getMax())
    {
        lose();
        return;
    }

    // All the possible tiles without the revealed tile
    std::vector<int> possibleIndexes(board.size());

    for (int i = 0; i < board.size(); i++)
        possibleIndexes.at(i) = i;
    possibleIndexes.erase(possibleIndexes.begin() + tile.getIndex());

    // Choose tiles randomly to turn in to bombs
    for (int i = 0; i < playSettings.getBombs(); i++)
    {
        int index = rand() % possibleIndexes.size();
        board.at(possibleIndexes.at(index)).setBomb(true);
        possibleIndexes.erase(possibleIndexes.begin() + index);
    }

    // Set the values of the tiles that are not bombs
    for (Tile& tile : board)
    {
        int value = 0;
        for (Tile* neighbor : tile.getNeighbors())
            if (neighbor->isBomb())
                value++;
        tile.setValue(value);
    }
}

/**
 * Executes when the game is won
 * 
 * Sends the player back to the pregame settings menu
 */
void Playing::win()
{
    Game::setState(new SettingsMenu(playSettings));
}

/**
 * Executes when the game is lost
 * 
 * Sends the player back to the pregame settings menu
 */
void Playing::lose()
{
    Game::setState(new SettingsMenu(playSettings));
}