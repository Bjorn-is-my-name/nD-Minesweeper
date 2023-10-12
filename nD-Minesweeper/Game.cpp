#include "Game.h"
#include "State.h"
#include <memory>
#include "Menu.h"
#include "Playing.h"
#include <iostream>
/**
 * Start everything needed for the application
 */
Game::Game()
{
    // Create a window with limited framerate
    window.create(sf::VideoMode(1920, 1080), "nD-Minesweeper", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Open the main menu
    State::set(std::make_unique<Menu>());

    // Create the gameloop
    run();
}

/**
 * Main gameloop
 */
void Game::run()
{
    // Keep running as long as the window exists and a gamestate is loaded
    while (window.isOpen() && State::isSet())
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

    while (window.pollEvent(event))
    {
        if (!State::isSet())
            return;

        auto& state = State::get();

        switch (event.type)
        {
        case sf::Event::Closed:
            quit();
            break;
        case sf::Event::KeyPressed:
            state.keyPressed(event.key.code);
            break;
        case sf::Event::KeyReleased:
            state.keyReleased(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            state.mousePressed(event.mouseButton);
            break;
        case sf::Event::MouseButtonReleased:
            state.mouseReleased(event.mouseButton);
            break;
        case sf::Event::MouseMoved:
            state.mouseMoved(event.mouseMove);
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
    if (!State::isSet())
        return;

    State::get().update();
}

/**
 * Calls 'Draw' of active gamestate
 */
void Game::draw()
{
    if (!State::isSet())
        return;

    State::get().draw(window);
    window.display();
}

/**
 * Closes the application
 */
void Game::quit()
{
    window.close();
}