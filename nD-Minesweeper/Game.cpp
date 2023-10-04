#include "Game.h"
#include "Menu.h"
#include "Playing.h"

// Initialize the static variable which holds the current gamestate
std::unique_ptr<GameState> Game::state;

/**
 * Start everything needed for the application
 */
Game::Game()
{
    // Create a window with limited framerate
    window.create(sf::VideoMode(1920, 1080), "nD-Minesweeper", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Open the main menu
    setState(new Menu());

    // Create the gameloop
    run();
}

/**
 * Main gameloop
 */
void Game::run()
{
    // Keep running as long as the window exists and a gamestate is loaded
    while (window.isOpen() && state)
    {
        handleEvents();
        update();
        draw();
    }

    quit();
}

/**
 * Handles events
 */
void Game::handleEvents()
{
    sf::Event event;

    // Check for events
    while (window.pollEvent(event))
    {
        if (!state)
            return;

        switch (event.type)
        {
        // On 'Closed' event, close the application
        case sf::Event::Closed:
            quit();
            break;
        // On 'KeyPressed' event, pass the key to the active gamestate
        case sf::Event::KeyPressed:
            state->keyPressed(event.key.code);
            break;
        // On 'MousePressed' event, pass the button and position to the active gamestate
        case sf::Event::MouseButtonPressed:
            state->mousePressed(event.mouseButton);
            break;
        // On 'MouseReleased' event, pass the button and position to the active gamestate
        case sf::Event::MouseButtonReleased:
            state->mouseReleased(event.mouseButton);
            break;
        default:
            break;
        }
    }
}

/**
 * Calls 'Update' of active gamestate
 */
void Game::update()
{
    if (!state)
        return;

    state->update();
}

/**
 * Calls 'Draw' of active gamestate
 */
void Game::draw()
{
    if (!state)
        return;

    state->draw(window);
    window.display();
}

/**
 * Closes the application
 */
void Game::quit()
{
    window.close();
}

/**
 * Changes the gamestate
 * 
 * @param newState new state to be loaded, 'null' if no state is specified
 */
void Game::setState(GameState* newState)
{
    state.reset(newState);
}