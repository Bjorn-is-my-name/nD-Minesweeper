#include "Game.h"
#include "Menu.h"
#include "Playing.h"

std::unique_ptr<GameState> Game::state;

Game::Game()
{
    window.create(sf::VideoMode(1920, 1080), "nD-Minesweeper", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    setState(new Menu());

    run();
}

void Game::run()
{
    while (window.isOpen() && state)
    {
        handleEvents();
        update();
        draw();
    }

    quit();
}

void Game::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (!state)
            return;

        switch (event.type)
        {
        case sf::Event::Closed:
            quit();
            break;
        case sf::Event::KeyPressed:
            state->keyPressed(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            state->mousePressed(event.mouseButton);
            break;
        case sf::Event::MouseButtonReleased:
            state->mouseReleased(event.mouseButton);
            break;
        default:
            break;
        }
    }
}

void Game::update()
{
    if (!state)
        return;

    state->update();
}

void Game::draw()
{
    if (!state)
        return;

    window.clear(sf::Color::Black);
    state->draw(window);
    window.display();
}

void Game::quit()
{
    window.close();
}

void Game::setState(GameState* newState)
{
    state.reset(newState);
}