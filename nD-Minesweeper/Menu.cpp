#include "Menu.h"
#include "State.h"
#include "SettingsMenu.h"
#include <exception>

/**
 * Initializes the main menu page
 * 
 * @throw std::runtime_error on file not found
 */
Menu::Menu()
{
    // Load files, exit game if unsuccesfull
    if (!textFont.loadFromFile("arial.ttf"))
        throw std::runtime_error("File arial.ttf not found");

    menutextLbl = Label(734, 200, "nD-Minesweeper", textFont, 60);

    // Play button to go to pregame menu
    playBtn = Button(860, 670, 200, 80, "Play", textFont, 40);
    playBtn.onClick = []() { State::set(std::make_unique<SettingsMenu>()); };
    buttons.push_back(&playBtn);

    // Exit button to exit the game
    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = []() { State::clear(); };
    buttons.push_back(&exitBtn);
}

/**
 * Updates the menu
 */
void Menu::update()
{

}

/**
 * Draws everything on the page
 *
 * @param window the window on which to draw the compontents
 */
void Menu::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);

    menutextLbl.draw(window);
    
    for (auto& button : buttons)
        button->draw(window);
}

/**
 * Handles key presses
 *
 * @param key key that is pressed
 */
void Menu::keyPressed(const sf::Keyboard::Key& key)
{
    switch (key)
    {
    case sf::Keyboard::Escape:
        State::clear();
        break;
    case sf::Keyboard::Enter:
        State::set(std::make_unique<SettingsMenu>());
        break;
    default:
        break;
    }
}

/**
 * Handles key releases
 * 
 * @param key the key that is released
 */
void Menu::keyReleased(const sf::Keyboard::Key& key)
{

}

/**
 * Handles mouse presses
 *
 * @param mouse holds the pressed mousebutton and mouse position
 */
void Menu::mousePressed(const sf::Event::MouseButtonEvent& mouse)
{

}

/**
 * Handles mouse releases
 *
 * @param mouse holds the released mousebutton and mouse position
 */
void Menu::mouseReleased(const sf::Event::MouseButtonEvent& mouse)
{
    // Check if a button is pressed
    for (auto& button : buttons)
    {
        if (button->pointOnRect(mouse.x, mouse.y))
        {
            button->onClick();
            break;
        }
    }
}

/**
 * Handles mouse moves
 * 
 * @param mouse holds the mouseposition
 */
void Menu::mouseMoved(const sf::Event::MouseMoveEvent& mouse)
{
    
}