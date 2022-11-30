#ifndef MAIN_H
#define MAIN_H

#include <raylib.h> // For the Vector2

// Defines
#define WINDOW_WIDTH 1920         // Width of the game window
#define HALF_WINDOW_WIDTH 960     // Half the window width for centering
#define WINDOW_HEIGHT 1080        // Height of the game window
#define FPS 60                    // Frames per second the game runs at
#define GAP_BETWEEN_CELLS 2       // Amount of pixels between every cell
#define GAP_BETWEEN_DIMENSIONS 5  // Amount of pixels between every nD dimension (set by extraSpaceThreshold variable)
#define HIGHLIGHT_BORDER_SIZE 3   // Pixel thickness of the highlight border
#define DEFAULT_CELLSIZE 40       // Default cellsize
#define MAX_CELLSIZE 100          // Maximum cellsize
#define MIN_CELLSIZE 5            // Minimum cellsize
#define MIN_CELLSIZE_FOR_TEXT 20  // Minimum cellsize for the cell its value to be shown
#define TEXT_CHAR_SPACING 2       // Amount of pixels between every character in drawn text
#define GUI_HEIGHT 200            // Height of the gui
#define EXTRA_SPACE_THRESHOLD 2   // The dimension at which axtra space
#define MINE -1                   // Value of a mine
#define OUT_OF_BOUNDS -1          // Value set if index is out of bounds or not relevant
#define LABELING_COLORS 24        // Amount of colors that can be used as a label
#define COLORS_PER_ROW 8          // Labeling colors per row
#define COLORS_PER_COLUMN 3       // Labeling colors per column
#define DEFAULT_FILTER_OPACITY 75 // Default filter opacity
#define GUI_TEXT_SIZE 20          // Text size for the gui
#define GUI_TITLE_TEXT_SIZE 70    // Text size for the settings and conrtols tab titles
#define TABS_TEXT_SIZE 30         // Text size for the gui in the settings and controls tab
#define TABS_BORDERSIZE 20        // Border size for the settings and controls tab
#define GUI_BORDERSIZE 10         // Border size for the game gui
#define NUM_OVERFLOW 99           // Max number shown in cell
#define NUM_OVERFLOW_TEXT "99+"   // The text that gets placed in the cell if the number is bigger then 99
#define MIN_FONT_SIZE 10          // Minimum font size for the cell value

// Node structure to store its data
template <typename T>
struct node
{
    T value;       // Value of the node
    node<T> *prev; // Address of the previous node
    node<T> *next; // Address of the previous node
};

// Doubly linked list class to store nodes
template <typename T>
class DoublyLinkedList
{
private:
    node<T> *head; // Fisrt node in the list
    int size;      // Size of the list

    // Get a new node
    node<T> *getNewNode(T value)
    {
        node<T> *newNode = new node<T>();
        newNode->value = value;
        newNode->next = NULL;
        newNode->prev = NULL;
        return newNode;
    }

public:
    // Create an empty list
    DoublyLinkedList()
    {
        head = NULL;
        size = 0;
    }

    // Add a node to the list
    void add(T value)
    {
        node<T> *n = getNewNode(value);
        size++;

        if (head == NULL)
        {
            head = n;
            return;
        }

        node<T> *last = head;

        while (last->next != NULL)
            last = last->next;

        last->next = n;
        n->prev = last;
    }

    // Get a node its value
    T &get(int index)
    {
        node<T> *n = head;

        while (index-- > 0)
            n = n->next;

        return n->value;
    }

    // Remove a node from the list
    void remove(int index)
    {
        node<T> *n = head;
        size--;

        if (index == 0)
        {
            if (head->next != NULL)
            {
                head = n->next;
                head->prev = NULL;
                delete n;
            }
            else
            {
                delete n;
                head = NULL;
            }
            return;
        }

        while (index-- > 0)
            n = n->next;

        if (n->next == NULL)
        {
            n->prev->next = NULL;
            delete n;
        }
        else
        {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            delete n;
        }
    }

    // Remove all nodes from the list
    void empty()
    {
        node<T> *n;
        size = 0;

        while (head != NULL)
        {
            n = head;
            head = head->next;
            delete n;
        }
    }

    // Get the size of the list
    int len()
    {
        return size;
    }
};

// Cell structure to store its information
struct cell
{
    Vector2 coords;     // Coordinates of the cell
    Vector2 drawCoords; // Coordinates at which the cell has to be drawn
    int value;          // Number of neighbors that are bombs
    bool visible;       // State of the cell (revealed or not)
    bool flagged;       // State of the cell (flagged or not)
    DoublyLinkedList<int> labels;

    // Set everything on initialization
    cell()
    {
        coords = {0, 0};
        drawCoords = {0, 0};
        value = 0;
        visible = false;
        flagged = false;
    }
};

void quit();                                      // Closes and deallocates everything before exiting the game
void update();                                    // Updates everything necesarry every frame
void revealCells();                               // Reveals the cell that is clicked and calls revealNeighbors
void revealNeighbors(int);                        // Reveals a clicked cell its neighbors
void flagCell();                                  // Flags or unflags the clicked cell
void labelCell();                                 // Label the clicked cell with the selected color
void unlabelCell();                               // Unlabel the clicked cell with the selected color
void draw();                                      // Draws everything on the screen
void drawBoard(int, int);                         // Draws all the cells
void drawSettings();                              // Draw the settings tab
void drawControls();                              // Draw the controls tab
unsigned char setOpacity(cell *);                 // Set the opacity based on the settings
bool cellOnScreen(int);                           // Checks if the cell is on the screen
void calculateDrawCoords();                       // Calculates all the coordinates the cells have to be draw at and calls drawBoard
void drawGui();                                   // Draws the gui
int getCellIndexFromMouse();                      // Returns the index of the cell the mouse is hovering on
bool mouseInCellBounds(int);                      // Checks if the mouse is between the borders of given cell
void highlightNeighbors();                        // Highlights all the neighbors of the cell the mouse is hovering over
void setupGame();                                 // Prepares everything for a new game
void setupBoard();                                // Sets up all the coordinates of the cells
int getTotalSize(int);                            // Calculates the total number of cells
int getNewTotalSize(int);                         // Calculates the total number of cells (only used for gui, when dimension is changed the max mines also needs to change)
int getSubtraction(int, int);                     // Calculates the amount of subtraction needed
void getNeighbors(int *, int, int *, int, int *); // Calculates all the given cell its neighbors based on the index
int *getTrueNeighbors(int);                       // Removes all the out of bounds neighbors gotton from getNeighbors
void generateMines(int);                          // Generates all the mines
void changeColorFilter();                         // Add or remove a color label from the color filter
void win();                                       // Handles everything after a winning
void gameover();                                  // Handles everything after a losing

// All drawing colors
Color colors[] = {
    Color{255, 0, 0, 255},
    Color{255, 136, 0, 255},
    Color{255, 255, 0, 255},
    Color{136, 0, 0, 255},
    Color{136, 68, 0, 255},
    Color{136, 136, 0, 255},
    Color{0, 255, 0, 255},
    Color{0, 255, 255, 255},
    Color{0, 0, 255, 255},
    Color{0, 136, 0, 255},
    Color{0, 136, 136, 255},
    Color{0, 0, 136, 255},
    Color{136, 0, 255, 255},
    Color{187, 0, 255, 255},
    Color{255, 0, 255, 255},
    Color{68, 0, 136, 255},
    Color{102, 0, 136, 255},
    Color{136, 0, 136, 255},
    Color{255, 0, 136, 255},
    Color{136, 136, 136, 255},
    Color{255, 255, 255, 255},
    Color{136, 0, 68, 255},
    Color{68, 68, 68, 255},
    Color{92, 64, 51, 255}};

// All gui object positions
Rectangle border1 = Rectangle{0, 0, 560, GUI_HEIGHT};
Rectangle border2 = Rectangle{550, 0, 700, GUI_HEIGHT};
Rectangle border3 = Rectangle{1240, 0, 475, GUI_HEIGHT};
Rectangle border4 = Rectangle{1705, 0, 215, GUI_HEIGHT};
Rectangle guiDimensionLabel = {40, 30, 70, 50};
Rectangle guiDimension = {40, 80, 75, 50};
Rectangle guiDimensionSizesDimensionLabel = {145, 30, 100, 50};
Rectangle guiDimensionSizesDimension = {145, 80, 100, 50};
Rectangle guiDimensionSizesValuesLabel = {260, 30, 50, 50};
Rectangle guiDimensionSizesValues = {260, 80, 50, 50};
Rectangle guiMinesLabel = {340, 30, 50, 50};
Rectangle guiMines = {340, 80, 100, 50};
Rectangle guiStart = {470, 80, 50, 50};
Rectangle guiMovingLabel = {610, 30, 60, 50};
Rectangle guiMoving = {610, 80, 50, 50};
Rectangle guiLabelingLabel = {700, 30, 60, 50};
Rectangle guiLabeling = {700, 80, 50, 50};
Rectangle guiSettings = {1730, 65, 165, 50};
Rectangle guiControls = {1730, 125, 165, 50};
Rectangle guiBack = {1785, 35, 100, 50};
Rectangle guiQuit = {1870, 25, 25, 25};
Rectangle guiFilterBoard = {WINDOW_WIDTH / 2 + 50, 390, 200, 50};
Rectangle guiFilterOpacity = {WINDOW_WIDTH / 2 + 50, 490, 200, 50};

#endif