#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <main.h>
#include <string>
#include <math.h>
h;
int dimension;
int *dimensionSizes; // width, length, height etc...
int totalCells;
cell *board;
int mines;
int flaggedMines;
int cellsize;
Vector2 drawingOffset;
bool playing = false;
bool movingMode = false;
bool drawingMode = false;
bool firstCellRevealed = false;
int selectedColor;
bool useFilterOnBoard = true;
unsigned char filterOpacity = DEFAULT_FILTER_OPACITY;

Vector2 mousePosition;
int newDimension = 1;
int newDimensionOld = 1;
int newDimensionSizesDimension = 1;
int *newDimensionSizesValues = new int[1]{1};
char newMinesInput[11] = "1";
int newMines = 1;

bool inSettings = false;
bool inControls = false;

DoublyLinkedList colorFilter;

main(void)
{
    // Set up the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Minesweeper nD");
    ToggleFullscreen();
    SetTargetFPS(FPS);

    // Set up the global gui style
    GuiLoadStyle("C:/Users/bjorn/Documents/CPlusPlusFiles/nD-Minesweeper/guiStyle.rgs");
    GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(VALUEBOX, TEXT_PADDING, 0);

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    quit();

    return 0;
}

void quit()
{
    CloseWindow();

    delete[] board;
    board = nullptr;
}

void update()
{
    if (inSettings)
    {
        if (IsKeyReleased(KEY_B))
            inSettings = false;
        return;
    }

    if (inControls)
    {
        if (IsKeyReleased(KEY_B))
            inControls = false;
        return;
    }

    if (playing)
    {
        if (IsKeyReleased(KEY_M))
            movingMode = !movingMode;

        if (IsKeyReleased(KEY_L))
            drawingMode = !drawingMode;

        // Change selected color
        // Go one up, if at top go back to bottom
        if (IsKeyPressed(KEY_W))
            selectedColor += (selectedColor % COLORS_PER_COLUMN == 0) ? COLORS_PER_COLUMN - 1 : -1;
        // Go one left, if at first of row go back to the end
        if (IsKeyPressed(KEY_A))
            selectedColor += (selectedColor < COLORS_PER_COLUMN) ? LABELING_COLORS - COLORS_PER_COLUMN : -COLORS_PER_COLUMN;
        // Go one down, if at bottom go back to top
        if (IsKeyPressed(KEY_S))
            selectedColor += (selectedColor % COLORS_PER_COLUMN == COLORS_PER_COLUMN - 1) ? -(COLORS_PER_COLUMN - 1) : 1;
        // Go one right, if at last of row go back to first
        if (IsKeyPressed(KEY_D))
            selectedColor += (selectedColor >= LABELING_COLORS - COLORS_PER_COLUMN) ? -(LABELING_COLORS - COLORS_PER_COLUMN) : COLORS_PER_COLUMN;

        // Add or remove selected color
        if (IsKeyPressed(KEY_F))
            changeColorFilter();

        // Zoom in or out based on scroll wheel movement
        int mouseWheelY = GetMouseWheelMoveV().y;
        if (mouseWheelY != 0)
        {
            if (mouseWheelY > 0 && cellsize < MAX_CELLSIZE)
                cellsize++;
            else if (mouseWheelY < 0 && cellsize > MIN_CELLSIZE)
                cellsize--;

            calculateDrawCoords();
        }

        if (!movingMode && !drawingMode)
        {
            // Reveal the clicked cell
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                // Never start on a mine
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

            // Flag or unflag the clicked cell
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
                flagCell();
        }
        else
        {
            if (movingMode)
            {
                // Move the board on dragging with right mouse down
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    Vector2 mouseDelta = GetMouseDelta();
                    drawingOffset.x += mouseDelta.x;
                    drawingOffset.y += mouseDelta.y;
                }
            }

            if (drawingMode)
            {
                // Label or unlabel the cell
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    labelCell();

                if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && !movingMode)
                    unlabelCell();
            }
        }
    }
    else
    {
        // Change dimension for setting dimension size
        if (IsKeyPressed(KEY_A))
            newDimensionSizesDimension--;

        if (IsKeyPressed(KEY_D))
            newDimensionSizesDimension++;
    }

    // Start or stop the game
    if (IsKeyPressed(KEY_SPACE))
    {
        if (playing)
            gameover();
        else
            setupGame();
    }

    // Open settings
    if (IsKeyPressed(KEY_X))
        inSettings = true;

    // Open controls
    if (IsKeyPressed(KEY_C))
        inControls = true;
}

void revealCells()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked
    if (cellIndex == OUT_OF_BOUNDS)
        return;

    cell *cell = &board[cellIndex];

    // Return if its already visible
    if (cell->visible)
        return;

    // Remove the flag and labels if present
    cell->visible = true;
    cell->flagged = false;
    cell->labels.empty();

    // If the cell is a mine, the player loses
    if (cell->value == MINE)
        gameover();
    // If the cell has no mines around it, reveal the neighboring cells
    else if (cell->value == 0)
        revealNeighbors(cellIndex);
}

void revealNeighbors(int cellIndex)
{
    // Reveal all the neighbors, recursivly call if the're not surrounded by any mines either
    int *neighbors = getTrueNeighbors(cellIndex);

    // Loop through all neighbors
    for (int i = pow(3, dimension) - 1; i >= 0; i--)
    {
        if (neighbors[i] != OUT_OF_BOUNDS && !board[neighbors[i]].visible)
        {
            cell *cell = &board[neighbors[i]];
            cell->visible = true;
            cell->flagged = false;
            cell->labels.empty();

            if (cell->value == 0)
                revealNeighbors(neighbors[i]);
        }
    }

    delete[] neighbors;
    neighbors = nullptr;
}

void flagCell()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked
    if (cellIndex == OUT_OF_BOUNDS)
        return;

    cell *cell = &board[cellIndex];

    // Return if the cell is visible
    if (cell->visible)
        return;

    // Flag or unflag the cell and update the flaggedMines counter if the cell was a mine
    if (cell->flagged)
    {
        cell->flagged = false;

        if (cell->value == MINE)
            flaggedMines--;
    }
    else
    {
        cell->flagged = true;

        // If the last mines is flagged, the player wins
        if (cell->value == MINE)
            if (++flaggedMines == mines)
                win();
    }
}

void labelCell()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked
    if (cellIndex == OUT_OF_BOUNDS)
        return;

    cell *cell = &board[cellIndex];

    // Return if the cell is visible
    if (cell->visible)
        return;

    int numOfLabels = cell->labels.len();

    // Return if the cell already has all the color labels
    if (numOfLabels == LABELING_COLORS)
        return;

    // Return if the cell is already labeled with the selected color
    for (int i = 0; i < numOfLabels; i++)
        if (cell->labels.get(i) == selectedColor)
            return;

    cell->labels.add(selectedColor);
}

void unlabelCell()
{
    int cellIndex = getCellIndexFromMouse();

    // Return if no cell is clicked
    if (cellIndex == OUT_OF_BOUNDS)
        return;

    cell *cell = &board[cellIndex];

    // Return if the cell is visible
    if (cell->visible)
        return;

    // If the cell has the color label, remove it
    for (int i = cell->labels.len() - 1; i >= 0; i--)
        if (cell->labels.get(i) == selectedColor)
            cell->labels.remove(i);
}

void draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    GuiSetStyle(DEFAULT, TEXT_SIZE, GUI_TEXT_SIZE);

    // Draw the settings tab
    if (inSettings)
    {
        drawSettings();
        EndDrawing();
        return;
    }

    // Draw the controls tab
    if (inControls)
    {
        drawControls();
        EndDrawing();
        return;
    }

    // Draw all the cells and highlight if hovered over
    if (dimension != NULL)
    {
        // Measure the max fontsize that makes the text fit in the cell
        int fontsize = MIN_FONT_SIZE;

        if (cellsize > MIN_CELLSIZE_FOR_TEXT)
        {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), NUM_OVERFLOW_TEXT, fontsize, TEXT_CHAR_SPACING);
            while (std::max(textSize.x, textSize.y) < cellsize - HIGHLIGHT_BORDER_SIZE)
                textSize = MeasureTextEx(GetFontDefault(), NUM_OVERFLOW_TEXT, ++fontsize, TEXT_CHAR_SPACING);
        }

        // Draw all the cells
        for (int i = 0; i < totalCells; i++)
            drawBoard(i, fontsize);

        if (playing)
            highlightNeighbors();
    }

    // Draw the gui for user input
    drawGui();

    EndDrawing();
}

void drawBoard(int cellIndex, int fontsize)
{
    // Check if the cell is on the screen
    if (!cellOnScreen(cellIndex))
        return;

    cell *cell = &board[cellIndex];

    // Set the cell its opacity based on the color filter
    unsigned char opacity = setOpacity(cell);

    // If the cell is visible, color it based on its value, else make it gray
    Color color = (cell->visible) ? Color{255, (unsigned char)(255 - std::min(cell->value / 2, 10) * 25), (unsigned char)(255 - std::min(cell->value / 2, 10) * 25), opacity} : Color{130, 130, 130, opacity};

    // Draw the cell
    DrawRectangle(
        cell->drawCoords.x + drawingOffset.x,
        cell->drawCoords.y + drawingOffset.y,
        cellsize,
        cellsize,
        color);

    // Precalculate in order to save time because of mulitple use (divisions are slow)
    int half = cellsize / 2;
    int quart = cellsize / 4;
    int tenth = cellsize / 10;

    // If the cell is flagged, draw a little flag
    if (cell->flagged)
    {
        DrawRectangle(
            cell->drawCoords.x + quart + drawingOffset.x,
            cell->drawCoords.y + drawingOffset.y,
            tenth + 0.5f,
            cellsize,
            BLACK);

        DrawTriangle(
            Vector2{
                cell->drawCoords.x + quart + tenth + drawingOffset.x,
                cell->drawCoords.y + drawingOffset.y},
            Vector2{
                cell->drawCoords.x + quart + tenth + drawingOffset.x,
                cell->drawCoords.y + half + drawingOffset.y},
            Vector2{
                cell->drawCoords.x + cellsize * 0.9f + drawingOffset.x,
                cell->drawCoords.y + quart + drawingOffset.y},
            RED);
    }

    // Only draw the value if the cell is visible and the value is not 0
    if (cell->visible && cellsize > MIN_CELLSIZE_FOR_TEXT && cell->value != 0)
    {
        std::string text = (cell->value > NUM_OVERFLOW) ? NUM_OVERFLOW_TEXT : std::to_string(cell->value);

        // Measure the text size to be able to center the value in the square
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontsize, TEXT_CHAR_SPACING);

        // Draw the value
        DrawTextEx(
            GetFontDefault(),
            text.c_str(),
            Vector2{
                cell->drawCoords.x + (half - textSize.x / 2) + drawingOffset.x,
                cell->drawCoords.y + (half - textSize.y / 2) + drawingOffset.y},
            fontsize,
            TEXT_CHAR_SPACING,
            BLACK);
    }
}

void drawSettings()
{
    // Draw the border
    DrawRectangleLinesEx(Rectangle{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, TABS_BORDERSIZE, GRAY);

    // Draw te title
    int textWidth = MeasureText("Settings", GUI_TITLE_TEXT_SIZE);
    DrawText(
        "Settings",
        HALF_WINDOW_WIDTH - textWidth / 2,
        50,
        GUI_TITLE_TEXT_SIZE,
        LIGHTGRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TABS_TEXT_SIZE);

    textWidth = MeasureText("Filter opacity used on board", TABS_TEXT_SIZE) + 50;
    DrawText(
        "Filter opacity used on board",
        HALF_WINDOW_WIDTH - textWidth,
        400,
        TABS_TEXT_SIZE,
        LIGHTGRAY);

    if (GuiButton(guiFilterBoard, useFilterOnBoard ? "On" : "Off"))
        useFilterOnBoard = !useFilterOnBoard;

    textWidth = MeasureText("Filter opacity", TABS_TEXT_SIZE) + 50;
    DrawText(
        "Filter opacity",
        HALF_WINDOW_WIDTH - textWidth,
        500,
        TABS_TEXT_SIZE,
        LIGHTGRAY);

    filterOpacity = GuiSlider(
        guiFilterOpacity,
        "0",
        "255",
        filterOpacity,
        0,
        255);

    if (GuiButton(guiQuit, ""))
        inSettings = false;
    DrawLine(WINDOW_WIDTH - 45, 30, WINDOW_WIDTH - 30, 45, LIGHTGRAY);
    DrawLine(WINDOW_WIDTH - 30, 30, WINDOW_WIDTH - 45, 45, LIGHTGRAY);
}

void drawControls()
{
    // Draw the border
    DrawRectangleLinesEx(Rectangle{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, TABS_BORDERSIZE, GRAY);

    // Draw the title
    int textWidth = MeasureText("Controls", GUI_TITLE_TEXT_SIZE);
    DrawText(
        "Controls",
        HALF_WINDOW_WIDTH - textWidth / 2,
        50,
        GUI_TITLE_TEXT_SIZE,
        LIGHTGRAY);

    // Draw all the controls
    textWidth = MeasureText("Mouse Right    \n", TABS_TEXT_SIZE);
    DrawText(
        "SPACE\n"
        "A\n"
        "D\n"
        "\n"
        "M\n"
        "Mouse Right\n"
        "\n"
        "L\n"
        "Mouse Left\n"
        "Mouse Right\n"
        "W\n"
        "A\n"
        "S\n"
        "D\n"
        "F\n"
        "\n"
        "X\n"
        "C\n"
        "B\n"
        "ESCAPE\n",
        HALF_WINDOW_WIDTH - 200 - textWidth,
        145,
        TABS_TEXT_SIZE,
        LIGHTGRAY);

    DrawText(
        "-    Play/Stop\n"
        "-    Previous dimension size\n"
        "-    Next dimension size\n"
        "\n"
        "-    Toggle Moving mode\n"
        "-    Hold and drag to move the board (overrides drawing mode)\n"
        "\n"
        "-    Toggle Drawing mode\n"
        "-    Label the cell with the selected color\n"
        "-    Remove the label with the selected color from the cell\n"
        "-    Switch to the color above the selected color\n"
        "-    Switch to the color left of the selected color\n"
        "-    Switch to the color below the selected color\n"
        "-    Switch to the color right of the selected color\n"
        "-    Add the selected color to the color filter\n"
        "\n"
        "-    Settings\n"
        "-    Controls\n"
        "-    Exit (exit the settings/controls tab)\n"
        "-    Quit (quit the game)\n",
        HALF_WINDOW_WIDTH - 200,
        145,
        TABS_TEXT_SIZE,
        LIGHTGRAY);

    if (GuiButton(guiQuit, ""))
        inControls = false;
    DrawLine(WINDOW_WIDTH - 45, 30, WINDOW_WIDTH - 30, 45, LIGHTGRAY);
    DrawLine(WINDOW_WIDTH - 30, 30, WINDOW_WIDTH - 45, 45, LIGHTGRAY);
}

unsigned char setOpacity(cell *cell)
{
    if (colorFilter.len() == 0)
        return 255;

    for (int i = colorFilter.len() - 1; i >= 0; i--)
    {
        for (int j = cell->labels.len() - 1; j >= 0; j--)
        {
            if (cell->labels.get(j) == colorFilter.get(i))
            {
                return (useFilterOnBoard) ? 255 : filterOpacity;
            }
        }
    }

    return (useFilterOnBoard) ? filterOpacity : 255;
}

bool cellOnScreen(int cellIndex)
{
    Vector2 coords = board[cellIndex].drawCoords;
    return coords.x + cellsize + drawingOffset.x > 0 && coords.x + drawingOffset.x < WINDOW_WIDTH && coords.y + cellsize + drawingOffset.y > GUI_HEIGHT && coords.y + drawingOffset.y < WINDOW_HEIGHT;
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

    for (int i = 0; i < totalCells; i++)
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
        cell *cell = &board[i];
        cell->drawCoords.x = cell->coords.x * (cellsize + GAP_BETWEEN_CELLS) + (x * GAP_BETWEEN_DIMENSIONS);
        cell->drawCoords.y = cell->coords.y * (cellsize + GAP_BETWEEN_CELLS) + (y * GAP_BETWEEN_DIMENSIONS);
    }
}

void drawGui()
{
    // Draw the background and borders
    DrawRectangle(0, 0, WINDOW_WIDTH, GUI_HEIGHT, BLACK);
    DrawRectangleLinesEx(border1, GUI_BORDERSIZE, GRAY);
    DrawRectangleLinesEx(border2, GUI_BORDERSIZE, GRAY);
    DrawRectangleLinesEx(border3, GUI_BORDERSIZE, GRAY);
    DrawRectangleLinesEx(border4, GUI_BORDERSIZE, GRAY);

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        mousePosition = GetMousePosition();

    if (playing)
        GuiDisable();

    // Draw all the gui objects and make set the editMode true if the object is clicked

    // Dimension
    GuiLabel(guiDimensionLabel, "Dimension");
    if (GuiValueBox(
            guiDimension,
            NULL,
            &newDimension,
            1,
            6,
            CheckCollisionPointRec(mousePosition, guiDimension)))
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
        CheckCollisionPointRec(mousePosition, guiDimensionSizesDimension));

    // DimensionSizes size
    GuiLabel(guiDimensionSizesValuesLabel, "Size");
    GuiValueBox(
        guiDimensionSizesValues,
        NULL,
        &newDimensionSizesValues[(newDimensionSizesDimension > 0 && newDimensionSizesDimension <= newDimension) ? newDimensionSizesDimension - 1 : ((newDimensionSizesDimension <= 0 || newDimension == 0) ? 0 : newDimension - 1)],
        1,
        10,
        CheckCollisionPointRec(mousePosition, guiDimensionSizesValues));

    // Mines
    GuiLabel(guiMinesLabel, "Mines");
    if (GuiTextBox(
            guiMines,
            newMinesInput,
            10,
            CheckCollisionPointRec(mousePosition, guiMines)))
    {
        // Convert from text to number
        int value = std::atoi(newMinesInput);
        int max = getNewTotalSize(newDimension);

        // If the number is too high reduce it to the max
        if (value > max)
            value = max;

        // Set the number of mines
        newMines = value;

        // Clear the text
        for (int i = 0; i < 10; i++)
            newMinesInput[i] = 0;

        // Reset the text
        if (value == 0)
            newMinesInput[0] = '0';
        else
        {
            int tmp = value;
            int i = 0;
            while (tmp > 0)
            {
                i++;
                tmp /= 10;
            }

            while (value > 0)
            {
                newMinesInput[--i] = value % 10 + '0';
                value /= 10;
            }
        }
    }

    GuiEnable();

    // Start button
    if (GuiButton(guiStart, playing ? "Stop" : "Play"))
    {
        if (playing)
            gameover();
        else
            setupGame();
    }

    if (!playing)
        GuiDisable();

    // Moving mode
    GuiLabel(guiMovingLabel, "Move");
    if (GuiButton(guiMoving, movingMode ? "On" : "Off"))
        movingMode = !movingMode;

    // Drawing mode
    GuiLabel(guiDrawingLabel, "Draw");
    if (GuiButton(guiDrawing, drawingMode ? "On" : "Off"))
        drawingMode = !drawingMode;

    // Color options
    for (int i = 0; i < COLORS_PER_ROW; i++)
    {
        for (int j = 0; j < COLORS_PER_COLUMN; j++)
        {
            int colorIndex = j + i * COLORS_PER_COLUMN;
            if (GuiButton(Rectangle{(float)(800 + i * 55), (float)(25 + j * 55), 40, 40}, ""))
                selectedColor = colorIndex;

            DrawRectangle(800 + i * 55, 25 + j * 55, 40, 40, (GuiGetState() == STATE_DISABLED) ? LIGHTGRAY : colors[colorIndex]);
            DrawRectangleLinesEx(Rectangle{(float)(800 + i * 55), (float)(25 + j * 55), 40, 40}, (colorIndex == selectedColor) ? 5 : 2, LIGHTGRAY);

            for (int k = 0; k < colorFilter.len(); k++)
            {
                if (colorFilter.get(k) == colorIndex)
                {
                    DrawText("f", 800 + i * 55 + 10, 25 + j * 55 + 5, 20, (GuiGetState() == STATE_DISABLED) ? GRAY : BLACK);
                    break;
                }
            }
        }
    }

    if (playing)
    {
        int cellIndex = getCellIndexFromMouse();

        if (cellIndex != OUT_OF_BOUNDS)
        {
            int size = board[cellIndex].labels.len();
            int i = 0;
            for (int j = 0; j < size; j++)
            {
                DrawRectangle(1265 + j % COLORS_PER_ROW * 55, 25 + i * 55, 40, 40, colors[board[cellIndex].labels.get(j)]);
                DrawRectangleLinesEx(Rectangle{(float)(1265 + j % COLORS_PER_ROW * 55), (float)(25 + i * 55), 40, 40}, 2, LIGHTGRAY);

                if ((j + 1) % COLORS_PER_ROW == 0)
                    i++;
            }
        }
    }

    GuiEnable();

    // Settings button
    if (GuiButton(guiSettings, "Settings"))
        inSettings = true;

    // Controls button
    if (GuiButton(guiControls, "Controls"))
        inControls = true;

    // Quit button
    if (GuiButton(guiQuit, ""))
        quit();
    DrawLine(WINDOW_WIDTH - 45, 30, WINDOW_WIDTH - 30, 45, LIGHTGRAY);
    DrawLine(WINDOW_WIDTH - 30, 30, WINDOW_WIDTH - 45, 45, LIGHTGRAY);
}

int getCellIndexFromMouse()
{
    // Get the index of the cell the mouse is hovering over
    if (GetMousePosition().y < GUI_HEIGHT)
        return OUT_OF_BOUNDS;

    mousePosition = GetMousePosition();

    for (int i = 0; i < totalCells; i++)
        if (mouseInCellBounds(i))
            return i;

    return OUT_OF_BOUNDS;
}

bool mouseInCellBounds(int cellIndex)
{
    // Check if the mouse positions is in the cell
    Vector2 coords = board[cellIndex].drawCoords;
    return mousePosition.x >= coords.x + drawingOffset.x && mousePosition.x <= coords.x + cellsize + GAP_BETWEEN_CELLS + drawingOffset.x && mousePosition.y >= coords.y + drawingOffset.y && mousePosition.y <= coords.y + cellsize + GAP_BETWEEN_CELLS + drawingOffset.y;
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
                cell *cell = &board[neighbors[i]];
                DrawRectangleLinesEx(
                    Rectangle{
                        cell->drawCoords.x + drawingOffset.x,
                        cell->drawCoords.y + drawingOffset.y,
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
    dimension = newDimension;

    delete[] dimensionSizes;
    dimensionSizes = new int[dimension];

    for (int i = 0; i < dimension; i++)
        dimensionSizes[i] = newDimensionSizesValues[i];

    totalCells = getTotalSize(dimension);

    delete[] board;
    board = new cell[totalCells];

    mines = newMines;
    flaggedMines = 0;
    cellsize = DEFAULT_CELLSIZE;
    drawingOffset.x = 50;
    drawingOffset.y = 50 + GUI_HEIGHT;
    selectedColor = 0;
    colorFilter.empty();
    firstCellRevealed = false;

    setupBoard();
    calculateDrawCoords();

    playing = true;
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

    for (int i = 0; i < totalCells; i++)
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
        cell *cell = &board[i];
        cell->coords.x = x - 1;
        cell->coords.y = y;
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
        if (neighbors[i] >= 0 && neighbors[i] < totalCells)
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
    for (int i = 0; i < totalCells; i++)
        freeSpots.add(i);

    if (mines < totalCells)
        freeSpots.remove(cellIndex);

    for (int i = 0; i < mines; i++)
    {
        // Set a random cell as mine
        int randomIndex = GetRandomValue(0, freeSpots.len() - 1);
        int randomCell = freeSpots.get(randomIndex);
        freeSpots.remove(randomIndex);

        board[randomCell].value = MINE;

        // Increment the value of all the cell its neighbors
        int *neighbors = getTrueNeighbors(randomCell);

        for (int j = pow(3, dimension) - 1; j >= 0; j--)
        {
            if (neighbors[j] != OUT_OF_BOUNDS)
            {
                cell *cell = &board[neighbors[j]];
                if (cell->value != MINE)
                    cell->value++;
            }
        }

        delete[] neighbors;
        neighbors = nullptr;
    }

    freeSpots.empty();
}

void changeColorFilter()
{
    // If the color is in the list, remove it, else add it
    for (int i = colorFilter.len() - 1; i >= 0; i--)
    {
        if (colorFilter.get(i) == selectedColor)
        {
            colorFilter.remove(i);
            return;
        }
    }

    colorFilter.add(selectedColor);
}

void win()
{
    playing = false;
    movingMode = false;
    drawingMode = false;
}

void gameover()
{
    playing = false;
    movingMode = false;
    drawingMode = false;
}