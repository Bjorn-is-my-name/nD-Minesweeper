#include "Menu.h"
#include "SettingsMenu.h"

Menu::Menu()
{
    // Load files, exit game if unsuccesfull
    if (!textFont.loadFromFile("arial.ttf"))
        Game::setState();

    menutextLbl = Label(734, 200, "nD-Minesweeper", textFont, 60);

    playBtn = Button(860, 670, 200, 80, "Play", textFont, 40);
    playBtn.onClick = []() {
        Game::setState(new SettingsMenu());
    };
    buttons.push_back(&playBtn);

    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = []() { Game::setState(); };
    buttons.push_back(&exitBtn);
}

void Menu::update()
{

}

void Menu::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    menutextLbl.draw(window);
    
    for (auto& button : buttons)
        button->draw(window);
}

void Menu::keyPressed(const sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Escape:
        Game::setState();
        break;
    case sf::Keyboard::Enter:
        Game::setState(new SettingsMenu());
        break;
    default:
        break;
    }
}

void Menu::mousePressed(const sf::Event::MouseButtonEvent event)
{

}

void Menu::mouseReleased(const sf::Event::MouseButtonEvent event)
{
    for (auto& button : buttons)
    {
        if (button->pointOnRect(event.x, event.y))
        {
            button->onClick();
            break;
        }
    }
}