#include "Menu.h"
#include "SettingsMenu.h"

/**
 * Initializes the main menu page
 */
Menu::Menu()
{
    // Load files, exit game if unsuccesfull
    if (!textFont.loadFromFile("arial.ttf"))
        Game::setState();

    menutextLbl = Label(734, 200, "nD-Minesweeper", textFont, 60);

    // Play button to go to pregame menu
    playBtn = Button(860, 670, 200, 80, "Play", textFont, 40);
    playBtn.onClick = []() {
        Game::setState(new SettingsMenu());
    };
    buttons.push_back(&playBtn);

    // Exit button to exit the game
    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = []() { Game::setState(); };
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
void Menu::keyPressed(const sf::Keyboard::Key key)
{
    switch (key)
    {
    // On 'Escape' exit the game
    case sf::Keyboard::Escape:
        Game::setState();
        break;
    // On 'Enter' go to the pregame menu
    case sf::Keyboard::Enter:
        Game::setState(new SettingsMenu());
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
void Menu::mousePressed(const sf::Event::MouseButtonEvent event)
{

}

/**
 * Handles mouse releases
 *
 * @param event mouse event which has released mousebutton and mouse position
 */
void Menu::mouseReleased(const sf::Event::MouseButtonEvent event)
{
    // Check if a button is pressed
    for (auto& button : buttons)
    {
        if (button->pointOnRect(event.x, event.y))
        {
            button->onClick();
            break;
        }
    }
}