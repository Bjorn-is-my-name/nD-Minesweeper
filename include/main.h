#ifndef MAIN_H
#define MAIN_H

#include <raylib.h> // For the Vector2

// Defines
#define WINDOW_WIDTH 1920        // Width of the game window
#define WINDOW_HEIGHT 1080       // Height of the game window
#define FPS 60                   // Frames per second the game runs at
#define GAP_BETWEEN_CELLS 2      // Amount of pixels between every cell
#define GAP_BETWEEN_DIMENSIONS 5 // Amount of pixels between every nD dimension (set by extraSpaceThreshold variable)
#define HIGHLIGHT_BORDER_SIZE 3  // Pixel thickness of the highlight border
#define DEFAULT_CELLSIZE 40      // Default cellsize
#define MAX_CELLSIZE 100         // Maximum cellsize
#define MIN_CELLSIZE 5           // Minimum cellsize
#define MIN_CELLSIZE_FOR_TEXT 20 // Minimum cellsize for the cell its value to be shown
#define TEXT_CHAR_SPACING 2      // Amount of pixels between every character in drawn text
#define GUI_HEIGHT 200           // Height of the gui
#define EXTRA_SPACE_THRESHOLD 2  // The dimension at which axtra space
#define MINE -1                  // Value of a mine
#define OUT_OF_BOUNDS -1         // Value set if index is out of bounds or not relevant

// Cell structure to store its information
struct cell
{
    Vector2 coords;     // Coordinates of the cell
    Vector2 drawCoords; // Coordinates at which the cell has to be drawn
    int value;          // Number of neighbors that are mines
    bool visible;       // State of the cell (revealed or not)
    bool flagged;       // State of the cell (flagged or not)

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

// Node structure to store its data
struct node
{
    int value;  // Value of the node
    node *prev; // Address of the previous node
    node *next; // Address of the previous node
};

// Doubly linked list class to store nodes
class DoublyLinkedList
{
private:
    node *head; // Fisrt node in the list
    int size;   // Size of the list

    // Get a new node
    node *getNewNode(int value)
    {
        node *newNode = new node();
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
    void add(int value)
    {
        node *n = getNewNode(value);
        size++;

        if (head == NULL)
        {
            head = n;
            return;
        }

        node *last = head;

        while (last->next != NULL)
            last = last->next;

        last->next = n;
        n->prev = last;
    }

    // Get a node its value
    int get(int index)
    {
        node *n = head;

        while (index-- > 0)
            n = n->next;

        return n->value;
    }

    // Remove a node from the list
    void remove(int index)
    {
        node *n = head;
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
        node *n;

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

void update();                                    // Updates everything necesarry every frame
void revealCells();                               // Reveals the cell that is clicked and calls revealNeighbors
void revealNeighbors(int);                        // Reveals a clicked cell its neighbors
void flagCell();                                  // Flags or unflags the cell that is clicked as a mine and decrements or increments the neighbors
void draw();                                      // Draws everything on the screen
void drawBoard(int, int);                         // Draws all the cells
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
void win();                                       // Handles everything after a winning
void gameover();                                  // Handles everything after a losing
int abs(int);                                     // Returns the positive version of the given number
int pow(int, int);                                // Returns the outcome of n to the power p

#endif