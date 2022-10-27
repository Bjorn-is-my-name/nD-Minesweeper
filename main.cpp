// Includes
#include <raylib.h>           // For all the graphical work
#define RAYGUI_IMPLEMENTATION // A define to let Raygui know its used together with Raylib and not used by itself
#include <raygui.h>           // For all the gui options
#include <main.h>             // For all the defines, typedefines, structs, classes and function presets
#include <string>             // For converting to const char * (c_str())

// Game variables
int dimension;                                                                                           // Dimension in which the game is played
int *dimensionSizes; /*minimum should be 1 but is 3 on dim 1 and 2 cuz still a bug in getTrueNeighbors*/ // Sizes of each dimension (width, length, depth etc...)
int cells;                                                                                               // Total number of cells
cell *board;                                                                                             // Board which stores all the cells
int mines;                                                                                               // Number of mines
int flaggedMines;                                                                                        // The number of mines flagged
int cellsize;                                                                                            // Changeable cellsize by scrolling
int xOffset;                                                                                             // X offset for moving the board around
int yOffset;                                                                                             // Y offset for moving the board around
bool playing = false;                                                                                    // To keep track of the game state
bool drawingMode = false;                                                                                // Switch between playing/flagging and moving/drawing
bool firstCellRevealed = false;                                                                          // Check when the first cell is clicked, generate mines after to always start with a 0

Vector2 mousePosition;                        // The positions of the mouse
int newDimension = 1;                         // The dimension for next game (changeable in the gui)
int newDimensionOld = 1;                      // The dimension for next game before changed by user (used to copy dimension sizes as the dimension changes)
int newDimensionSizesDimension = 1;           // The dimension of which the size is changeable (changeable in the gui)
int *newDimensionSizesValues = new int[1]{1}; // The size of the dimension selected (changeable in the gui)
int newMines = 1;                             // the number of mines for next game

int main(void)
{
    // Set up the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Minesweeper nD");
    ToggleFullscreen();
    SetTargetFPS(FPS);

    // Game loop
    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    // If the game is exited, close everything down
    CloseWindow();

    // Deallocate all the memory
    delete[] board;
    board = nullptr;

    return 0;
}

void update()
{
    // Only update if the mouse is outside of the gui
    if (!playing && GetMousePosition().y > GUI_HEIGHT)
        return;

    // If the mouse wheel is moved, zoom in or out and recalculate the drawing coords
    Vector2 mouseWheelMovement = GetMouseWheelMoveV();
    if (mouseWheelMovement.y != 0)
    {
        if (mouseWheelMovement.y > 0 && cellsize < MAX_CELLSIZE)
            cellsize++;
        else if (mouseWheelMovement.y < 0 && cellsize > MIN_CELLSIZE)
            cellsize--;

        calculateDrawCoords();
    }

    // Turn drawing mode on or off
    if (playing && IsKeyReleased(KEY_D))
        drawingMode = !drawingMode;

    if (drawingMode)
    {
        // Make the board move when holding right click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mouseDelta = GetMouseDelta();
            xOffset += mouseDelta.x;
            yOffset += mouseDelta.y;
        }
    }
    else
    {
        // Flag the clicked cell
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            flagCell();

        // Reveal the clicked cell
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (!firstCellRevealed)
            {
                int cellIndex = getCellIndexFromMouse();
                if (cellIndex != OUT_OF_BOUNDS)
                {
                    generateMines(cellIndex);
                    firstCellRevealed = true;
                }
            }

            revealCells();
        }
    }
}

void revealCells()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked or it is already visible
    if (cellIndex == OUT_OF_BOUNDS || board[cellIndex].visible)
        return;

    // If the cell is flagged, remove the flag
    board[cellIndex].visible = true;
    board[cellIndex].flagged = false;

    // If the cell is a mine, the player loses
    if (board[cellIndex].value == MINE)
        gameover();
    // If the cell has no neighbors that are mines, reveal them
    else if (board[cellIndex].value == 0)
        revealNeighbors(cellIndex);
}

void revealNeighbors(int cellIndex)
{
    // Reveal all the cell its neighbors (only called when cell value is 0) and recursively call if a neighbor is also 0
    int *neighbors = getTrueNeighbors(cellIndex);

    for (int i = pow(3, dimension) - 1; i >= 0; i--)
    {
        if (neighbors[i] != OUT_OF_BOUNDS && !board[neighbors[i]].visible)
        {
            board[neighbors[i]].visible = true;

            if (board[neighbors[i]].value == 0)
                revealNeighbors(neighbors[i]);
        }
    }

    delete[] neighbors;
    neighbors = nullptr;
}

void flagCell()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked or it is already visible
    if (cellIndex == OUT_OF_BOUNDS || board[cellIndex].visible)
        return;

    // Flag or unflag the cell and update the flaggedMines counter if the cell was a mine
    if (board[cellIndex].flagged)
    {
        board[cellIndex].flagged = false;

        if (board[cellIndex].value == MINE)
            flaggedMines--;
    }
    else
    {
        board[cellIndex].flagged = true;

        // If the last mines is flagged, the player wins
        if (board[cellIndex].value == MINE)
            if (++flaggedMines == mines)
                win();
    }
}

void draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    // Draw all the cells and highlight if hovered over
    if (dimension != NULL)
    {
        // Set the font size so the value fits in the cell
        int fontsize = 1;

        if (cellsize > MIN_CELLSIZE_FOR_TEXT)
        {
            // Increase the font size until the text no longer fits
            Vector2 textSize = MeasureTextEx(GetFontDefault(), "99+", fontsize, TEXT_CHAR_SPACING);
            while (std::max(textSize.x, textSize.y) < cellsize - HIGHLIGHT_BORDER_SIZE)
                textSize = MeasureTextEx(GetFontDefault(), "99+", ++fontsize, TEXT_CHAR_SPACING);
        }

        // Draw all the cells
        for (int i = 0; i < cells; i++)
            drawBoard(i, fontsize);

        highlightNeighbors();
    }

    // Draw the gui for user input
    drawGui();

    // Show if drawingMode is on or off
    std::string text = drawingMode ? "Drawing (D): On" : "Drawing (D): Off";
    DrawText(text.c_str(), 1500, 100, 20, WHITE);

    EndDrawing();
}

void drawBoard(int cellIndex, int fontsize)
{
    // Check if the cell is on the screen
    if (!cellOnScreen(cellIndex))
        return;

    // If the cell is visible color it based on its value, else make it gray
    Color color = (board[cellIndex].visible) ? color = {255, (unsigned char)(255 - std::min(board[cellIndex].value / 2, 10) * 25), (unsigned char)(255 - std::min(board[cellIndex].value / 2, 10) * 25), 255} : GRAY;

    // Draw the cell
    DrawRectangle(
        board[cellIndex].drawCoords.x + xOffset,
        board[cellIndex].drawCoords.y + yOffset,
        cellsize,
        cellsize,
        color);

    // If the cell is flagged, draw a little flag
    if (board[cellIndex].flagged)
    {
        DrawRectangle(
            board[cellIndex].drawCoords.x + cellsize / 4 + xOffset,
            board[cellIndex].drawCoords.y + yOffset,
            cellsize / 10 + 0.5f,
            cellsize,
            BLACK);

        DrawTriangle(
            Vector2{
                board[cellIndex].drawCoords.x + cellsize / 4 + cellsize / 10 + xOffset,
                board[cellIndex].drawCoords.y + yOffset},
            Vector2{
                board[cellIndex].drawCoords.x + cellsize / 4 + cellsize / 10 + xOffset,
                board[cellIndex].drawCoords.y + cellsize / 2 + yOffset},
            Vector2{
                board[cellIndex].drawCoords.x + cellsize * 0.9f + xOffset,
                board[cellIndex].drawCoords.y + cellsize / 4 + yOffset},
            RED);
    }

    // Only draw the value if the cell is visible and the value is not 0
    if (board[cellIndex].visible && cellsize > MIN_CELLSIZE_FOR_TEXT && board[cellIndex].value != 0)
    {
        std::string text = (board[cellIndex].value > 99) ? "99+" : std::to_string(board[cellIndex].value);

        // Measure the text size to be able to center the value in the square
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontsize, TEXT_CHAR_SPACING);

        // Draw the value
        DrawTextEx(
            GetFontDefault(),
            text.c_str(),
            Vector2{
                board[cellIndex].drawCoords.x + ((cellsize / 2) - (textSize.x / 2)) + xOffset,
                board[cellIndex].drawCoords.y + ((cellsize / 2) - (textSize.y / 2)) + yOffset},
            fontsize,
            TEXT_CHAR_SPACING,
            BLACK);
    }
}

bool cellOnScreen(int cellIndex)
{
    return board[cellIndex].drawCoords.x + cellsize + xOffset > 0 && board[cellIndex].drawCoords.x + xOffset < WINDOW_WIDTH && board[cellIndex].drawCoords.y + cellsize + yOffset > GUI_HEIGHT && board[cellIndex].drawCoords.y + yOffset < WINDOW_HEIGHT;
}

void calculateDrawCoords()
{
    int x = 0;
    int y = 0;

    // Create a copy of the dimensionSizes to use for looping (set all sizes 1 lower except the first)
    int *sizes = new int[dimension];
    sizes[0] = dimensionSizes[0];

    for (int i = 1; i < dimension; i++)
        sizes[i] = dimensionSizes[i] - 1;

    for (int i = 0; i < cells; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            // If the size is 0, reset it and check the next size
            if (sizes[j] == 0)
                sizes[j] = dimensionSizes[j] - 1;
            else
            {
                sizes[j]--;

                // Check if the dimension is above the threshold
                if (j > EXTRA_SPACE_THRESHOLD - 1)
                {
                    // Check if the dimension is even or uneven and change the x and y based on the dimension
                    if (j % 2)
                    {
                        y++;

                        // Only subtract if the dimension is high enough
                        if (j > EXTRA_SPACE_THRESHOLD)
                            x -= getSubtraction(EXTRA_SPACE_THRESHOLD, j) - 1;
                    }
                    else
                    {
                        x++;

                        // Only subtract if the dimension is high enough
                        if (j > EXTRA_SPACE_THRESHOLD)
                            y -= getSubtraction(EXTRA_SPACE_THRESHOLD + 1, j) - 1;
                    }
                }

                break;
            }
        }

        // Set the drawing coords
        board[i].drawCoords.x = board[i].coords.x * (cellsize + GAP_BETWEEN_CELLS) + (x * GAP_BETWEEN_DIMENSIONS);
        board[i].drawCoords.y = board[i].coords.y * (cellsize + GAP_BETWEEN_CELLS) + (y * GAP_BETWEEN_DIMENSIONS);
    }
}

void drawGui()
{
    // Draw the border around the gui
    DrawRectangleLinesEx(Rectangle{0, 0, WINDOW_WIDTH, GUI_HEIGHT}, 10, GRAY);

    // All gui object positions
    Rectangle guiDimensionLabel = {200, 30, 50, 50};
    Rectangle guiDimension = {200, 80, 50, 50};
    Rectangle guiDimensionSizesDimensionLabel = {400, 30, 100, 50};
    Rectangle guiDimensionSizesDimension = {400, 80, 100, 50};
    Rectangle guiDimensionSizesValuesLabel = {550, 30, 50, 50};
    Rectangle guiDimensionSizesValues = {550, 80, 50, 50};
    Rectangle guiMinesLabel = {700, 30, 50, 50};
    Rectangle guiMines = {700, 80, 50, 50};
    Rectangle guiStart = {800, 50, 50, 50};

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        mousePosition = GetMousePosition();

    // Draw all the gui objects and make set the editMode true if the object is clicked

    // Dimension
    GuiLabel(guiDimensionLabel, "Dimension");
    if (GuiValueBox(
            guiDimension,
            "",
            &newDimension,
            1,
            INT_MAX,
            !playing && CheckCollisionPointRec(mousePosition, guiDimension)))
    {
        // If the dimension is changed, update dimensionSizes but keep already set values intact
        if (newDimension > 0)
        {
            // Store the old sizes
            int *tempDimensionSizes = new int[newDimensionOld];

            for (int i = 0; i < newDimensionOld; i++)
                tempDimensionSizes[i] = newDimensionSizesValues[i];

            // Deallocate the old sizes array and create a new one
            delete[] newDimensionSizesValues;
            newDimensionSizesValues = new int[newDimension];

            // Copy all the old elements over, set the new sizes to 1 (only when dimension is increased)
            for (int i = 0; i < newDimension; i++)
            {
                if (i < newDimensionOld)
                    newDimensionSizesValues[i] = tempDimensionSizes[i];
                else
                    newDimensionSizesValues[i] = 1;
            }

            delete[] tempDimensionSizes;
            tempDimensionSizes = nullptr;
            // Remember the dimension for next change
            newDimensionOld = newDimension;
        }
    }

    // DimensionSizes dimension
    GuiLabel(guiDimensionSizesDimensionLabel, "Dimension");
    GuiSpinner(
        guiDimensionSizesDimension,
        "",
        &newDimensionSizesDimension,
        1,
        (newDimension > 0) ? newDimension : 1,
        !playing && CheckCollisionPointRec(mousePosition, guiDimensionSizesDimension));

    // DimensionSizes size
    GuiLabel(guiDimensionSizesValuesLabel, "Size");
    GuiValueBox(
        guiDimensionSizesValues,
        "",
        &newDimensionSizesValues[(newDimensionSizesDimension > 0 && newDimensionSizesDimension <= newDimension) ? newDimensionSizesDimension - 1 : ((newDimensionSizesDimension <= 0 || newDimension == 0) ? 0 : newDimension - 1)],
        1,
        INT_MAX,
        !playing && CheckCollisionPointRec(mousePosition, guiDimensionSizesValues));

    // Mines
    GuiLabel(guiMinesLabel, "Mines");
    GuiValueBox(
        guiMines,
        "",
        &newMines,
        1,
        getNewTotalSize(newDimension),
        !playing && CheckCollisionPointRec(mousePosition, guiMines));

    // Start button
    if (GuiButton(guiStart, playing ? "Stop" : "Start"))
    {
        if (playing)
            gameover();
        else
            setupGame();
    }
}

int getCellIndexFromMouse()
{
    // Get the index of the cell the mouse is hovering over
    if (GetMousePosition().y < GUI_HEIGHT)
        return -1;

    mousePosition = GetMousePosition();
    int cellIndex = OUT_OF_BOUNDS;

    for (int i = 0; i < cells; i++)
    {
        if (mouseInCellBounds(i))
        {
            cellIndex = i;
            break;
        }
    }

    return cellIndex;
}

bool mouseInCellBounds(int cellIndex)
{
    // Check if the mouse positions is in the cell
    return mousePosition.x >= board[cellIndex].drawCoords.x + xOffset && mousePosition.x <= board[cellIndex].drawCoords.x + cellsize + GAP_BETWEEN_CELLS + xOffset && mousePosition.y >= board[cellIndex].drawCoords.y + yOffset && mousePosition.y <= board[cellIndex].drawCoords.y + cellsize + GAP_BETWEEN_CELLS + yOffset;
}

void highlightNeighbors()
{
    // Highlight all neighbors of the cell hovered over
    int cellIndex = getCellIndexFromMouse();

    if (cellIndex != OUT_OF_BOUNDS)
    {
        int *neighbors = getTrueNeighbors(cellIndex);

        for (int i = pow(3, dimension) - 1; i >= 0; i--)
        {
            if (neighbors[i] != OUT_OF_BOUNDS && cellOnScreen(neighbors[i]))
            {
                DrawRectangleLinesEx(
                    Rectangle{
                        board[neighbors[i]].drawCoords.x + xOffset,
                        board[neighbors[i]].drawCoords.y + yOffset,
                        (float)cellsize,
                        (float)cellsize},
                    HIGHLIGHT_BORDER_SIZE,
                    SKYBLUE);
            }
        }

        delete[] neighbors;
        neighbors = nullptr;
    }
}

void setupGame()
{
    // Set everything up for a new game
    playing = true;
    dimension = newDimension;

    delete[] dimensionSizes;
    dimensionSizes = new int[dimension];

    for (int i = 0; i < dimension; i++)
        dimensionSizes[i] = newDimensionSizesValues[i];

    cells = getTotalSize(dimension);

    delete[] board;
    board = new cell[cells];

    mines = newMines;
    flaggedMines = 0;
    cellsize = DEFAULT_CELLSIZE;
    xOffset = 50;
    yOffset = 50 + GUI_HEIGHT;
    drawingMode = false;

    setupBoard();
    calculateDrawCoords();
}

void setupBoard()
{
    int x = 0;
    int y = 0;

    // Create a copy of the dimensionSizes to use for looping (set all sizes 1 lower except the first)
    int *sizes = new int[dimension];
    sizes[0] = dimensionSizes[0];

    for (int i = 1; i < dimension; i++)
        sizes[i] = dimensionSizes[i] - 1;

    for (int i = 0; i < cells; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            // If the size is 0, reset it and check the next size
            if (sizes[j] == 0)
                sizes[j] = dimensionSizes[j] - 1;
            else
            {
                sizes[j]--;

                // Check if the dimension is even or uneven and change the x and y based on the dimension
                if (j % 2)
                {
                    y++;
                    x -= getSubtraction(0, j) - 1;
                }
                else
                {
                    x++;
                    // Only subtract if the dimension is higher then 1
                    if (j > 0)
                        y -= getSubtraction(1, j) - 1;
                }

                break;
            }
        }

        // Set the cell its coords
        board[i].coords.x = x - 1;
        board[i].coords.y = y;
    }
}

int getTotalSize(int n)
{
    // Multiply all the dimension sizes to get the total number of cells
    int totalSize = dimensionSizes[0];

    for (int i = 1; i < n; i++)
        totalSize *= dimensionSizes[i];

    return totalSize;
}

int getNewTotalSize(int n)
{
    // Multiply all the new dimension sizes to get the total number of cells
    if (n == 0)
        return newMines;

    int totalSize = newDimensionSizesValues[0];

    for (int i = 1; i < n; i++)
        totalSize *= newDimensionSizesValues[i];

    // If 0 then a dimension value is being changed, keep the old value
    return (totalSize < 1) ? newMines : totalSize;
}

int getSubtraction(int start, int stop)
{
    // Get the amount of subtraction needed to correctly format the cells
    int subtraction = dimensionSizes[start];

    for (int i = start + 2; i < stop; i += 2)
        subtraction *= dimensionSizes[i];

    return subtraction;
}

void getNeighbors(int *neighbors, int currentDimension, int *dimensionValues, int cellIndex, int *counter)
{
    // Get all the neighbors of the given cell
    if (currentDimension == 0)
    {
        int index = cellIndex + dimensionValues[0];
        for (int i = 1; i < dimension; i++)
            index += dimensionValues[i] * getTotalSize(i);

        neighbors[(*counter)++] = index;
        return;
    }

    // If the dimension is higher then 1, recursively check the other dimensions
    for (int i = -1; i < 2; i++)
    {
        dimensionValues[currentDimension - 1] = i;
        getNeighbors(neighbors, currentDimension - 1, dimensionValues, cellIndex, counter);
    }
}

int *getTrueNeighbors(int cellIndex)
{
    // Get all the true neightbors (the ones that aren't out of bounds)
    int counter = 0;
    int *dimensionValues = new int[dimension];
    int *neighbors = new int[(int)pow(3, dimension)];

    getNeighbors(neighbors, dimension, dimensionValues, cellIndex, &counter);

    int *trueNeighbors = new int[counter];
    for (int i = 0; i < counter; i++)
        trueNeighbors[i] = OUT_OF_BOUNDS;

    for (int i = 0; i < counter; i++)
    {
        if (neighbors[i] >= 0 && neighbors[i] < cells)
        {
            bool outOfBounds = false;
            int threshold = 1;

            for (int j = 1; j < dimension; j++)
            {
                if (abs(cellIndex % getTotalSize(j) - neighbors[i] % getTotalSize(j)) > threshold)
                {
                    outOfBounds = true;
                    break;
                }
                threshold += getTotalSize(j);
            }

            if (!outOfBounds)
                trueNeighbors[i] = neighbors[i];
        }
    }

    delete[] dimensionValues;
    dimensionValues = nullptr;
    delete[] neighbors;
    neighbors = nullptr;
    return trueNeighbors;
}

void generateMines(int cellIndex)
{
    // Generate all the mines

    // Keep track of the free spots
    DoublyLinkedList freeSpots;
    for (int i = 0; i < cells; i++)
        freeSpots.add(i);

    if (mines < cells)
        freeSpots.remove(cellIndex);

    for (int i = 0; i < mines; i++)
    {
        // Set a random cell as mine
        int cell = GetRandomValue(0, freeSpots.len() - 1);
        int randomCell = freeSpots.get(cell);
        freeSpots.remove(cell);

        board[randomCell].value = MINE;

        // Increment the value of all the cell its neighbors
        int *neighbors = getTrueNeighbors(randomCell);

        for (int j = pow(3, dimension) - 1; j >= 0; j--)
        {
            if (neighbors[j] != OUT_OF_BOUNDS)
            {
                if (board[neighbors[j]].value != MINE)
                {
                    board[neighbors[j]].value++;
                }
            }
        }

        delete[] neighbors;
        neighbors = nullptr;
    }

    freeSpots.empty();
}

void win()
{
    playing = false;
    drawingMode = false;
    firstCellRevealed = false;
}

void gameover()
{
    playing = false;
    drawingMode = false;
    firstCellRevealed = false;
}

int abs(int n)
{
    // Return the positive version of the number
    return (n < 0) ? n * -1 : n;
}

int pow(int n, int p)
{
    // Return n to the power of p
    int o = n;
    while (--p > 0)
        o *= n;
    return o;
}