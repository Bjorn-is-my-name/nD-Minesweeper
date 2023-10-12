#include "SettingsMenu.h"
#include "State.h"
#include "Menu.h"
#include "Playing.h"
#include <exception>

/**
 * Initializes the pregame settings page
 * 
 * @param settings previous settings if returning from game, else default settings
 * @throw std::runtime_error on file not found
 */
SettingsMenu::SettingsMenu(const PlaySettings& settings)
    : playSettings(settings)
{
    // Load files, exit game if unsuccesfull
    if (!textFont.loadFromFile("arial.ttf"))
        throw std::runtime_error("File arial.ttf not found");

    // Menu text
    menutextLbl = Label(766, 200, "Game Settings", textFont, 60);
    labels.push_back(&menutextLbl);

    // Dimensions setting
    dimLbl = Label(600, 408, "Dimensions:", textFont, 30);
    labels.push_back(&dimLbl);

    dimTt = ToolTip(dimLbl.getX() - 40, dimLbl.getY() + dimLbl.getHeight() / 2 - 10, 20, 20, "Dimension to play in.", textFont, 18);
    tooltips.push_back(&dimTt);

    dimVB = ValueBox(dimLbl.getX() + dimLbl.getWidth() + 20, dimLbl.getY() - dimLbl.getHeight() / 2, 50, 50, playSettings.getDimensions(), 1, 99, textFont, 30);
    dimVB.onDeselect = [&]() {
        // Update the value, if value is below min, set value to min
        int value = dimVB.getValue();
        int min = dimVB.getMin();
        
        if (value < min)
        {
            playSettings.setDimensions(min);
            dimVB.setValue(min);
        }
        else
            playSettings.setDimensions(value);

        // Update max number of bombs allowed in game
        bombsVB.setMax(playSettings.getMax());
    };
    valueBoxes.push_back(&dimVB);

    // Dimension sizes setting
    dimSizeLbl = Label(600, 609, "Size:", textFont, 30);
    labels.push_back(&dimSizeLbl);

    dimSizeIdxLbl = Label(dimSizeLbl.getX() + dimSizeLbl.getWidth() + 62, dimSizeLbl.getY() - dimSizeLbl.getHeight() / 2 - 30, "(1)", textFont, 18);
    labels.push_back(&dimSizeIdxLbl);

    dimSizeTt = ToolTip(dimSizeLbl.getX() - 40, dimSizeLbl.getY() + dimSizeLbl.getHeight() / 2 - 10, 20, 20, "Size for the selected dimension.", textFont, 18);
    tooltips.push_back(&dimSizeTt);

    dimSizeVB = ValueBox(dimSizeLbl.getX() + dimSizeLbl.getWidth() + 50, dimSizeLbl.getY() - dimSizeLbl.getHeight() / 2, 50, 50, playSettings.getDimensionSize(selectedDimSize), 1, 99, textFont, 30);
    dimSizeVB.onDeselect = [&]() {
        // Update the value, if value is below min, set value to min
        int value = dimSizeVB.getValue();
        int min = dimSizeVB.getMin();

        if (value < min)
        {
            playSettings.setDimensionSize(selectedDimSize, min);
            dimSizeVB.setValue(min);
        }
        else
            playSettings.setDimensionSize(selectedDimSize, value);
        
        // Update the max number of bombs allowed in game
        bombsVB.setMax(playSettings.getMax());
    };
    valueBoxes.push_back(&dimSizeVB);

    dimSizeLeftBtn = Button(dimSizeVB.getX() - 30, dimSizeVB.getY(), 20, 50, "<", textFont, 30);
    dimSizeLeftBtn.onClick = [&]() {
        // Go to dimension size setting of a lower dimension (if not lowest)
        if (selectedDimSize > 0)
        {
            selectedDimSize--;
            dimSizeVB.setValue(playSettings.getDimensionSize(selectedDimSize));
            dimSizeIdxLbl.setText("(" + std::to_string(selectedDimSize + 1) + ')');
        }
    };
    buttons.push_back(&dimSizeLeftBtn);

    dimSizeRightBtn = Button(dimSizeVB.getX() + dimSizeVB.getWidth() + 10, dimSizeVB.getY(), 20, 50, ">", textFont, 30);
    dimSizeRightBtn.onClick = [&]() {
        // Go to dimension size setting of a higher dimension (if not highest)
        if (selectedDimSize < playSettings.getDimensions() - 1)
        {
            selectedDimSize++;
            dimSizeVB.setValue(playSettings.getDimensionSize(selectedDimSize));
            dimSizeIdxLbl.setText("(" + std::to_string(selectedDimSize + 1) + ')');
        }
    };
    buttons.push_back(&dimSizeRightBtn);

    // Bombs setting
    bombsLbl = Label(1000, 408, "Bombs:", textFont, 30);
    labels.push_back(&bombsLbl);

    bombsTt = ToolTip(bombsLbl.getX() - 40, bombsLbl.getY() + bombsLbl.getHeight() / 2 - 10, 20, 20, "Amount of bombs.", textFont, 18);
    tooltips.push_back(&bombsTt);

    bombsVB = ValueBox(bombsLbl.getX() + bombsLbl.getWidth() + 20, bombsLbl.getY() - bombsLbl.getHeight() / 2, 50, 50, playSettings.getBombs(), 0, playSettings.getMax(), textFont, 30);
    bombsVB.onDeselect = [&]() {
        // Update the value, if value is below min, set value to min
        int value = bombsVB.getValue();
        int min = bombsVB.getMin();

        if (value < min)
        {
            playSettings.setBombs(min);
            bombsVB.setValue(min);
        }
        else
            playSettings.setBombs(value);
    };
    valueBoxes.push_back(&bombsVB);

    // Extra space setting
    extraSpaceLbl = Label(1000, 609, "Space:", textFont, 30);
    labels.push_back(&extraSpaceLbl);

    extraSpaceIdxLbl = Label(extraSpaceLbl.getX() + extraSpaceLbl.getWidth() + 62, extraSpaceLbl.getY() - extraSpaceLbl.getHeight() / 2 - 30, "(1)", textFont, 18);
    labels.push_back(&extraSpaceIdxLbl);

    extraSpaceTt = ToolTip(extraSpaceLbl.getX() - 40, extraSpaceLbl.getY() + extraSpaceLbl.getHeight() / 2 - 10, 20, 20, "Space (in tiles) between each dimension.", textFont, 18);
    tooltips.push_back(&extraSpaceTt);

    extraSpaceVB = ValueBox(extraSpaceLbl.getX() + extraSpaceLbl.getWidth() + 50, extraSpaceLbl.getY() - extraSpaceLbl.getHeight() / 2, 50, 50, playSettings.getExtraSpace(selectedExtraSpace), 0, 99, textFont, 30);
    extraSpaceVB.onDeselect = [&]() {
        // Update the value, if value is below min, set value to min
        int value = extraSpaceVB.getValue();
        int min = extraSpaceVB.getMin();
        
        if (value < min)
        {
            playSettings.setExtraSpace(selectedExtraSpace, min);
            extraSpaceVB.setValue(min);
        }
        else
            playSettings.setExtraSpace(selectedExtraSpace, value);
    };
    valueBoxes.push_back(&extraSpaceVB);

    extraSpaceLeftBtn = Button(extraSpaceVB.getX() - 30, extraSpaceVB.getY(), 20, 50, "<", textFont, 30);
    extraSpaceLeftBtn.onClick = [&]() {
        // Go to extra space setting of a lower dimension (if not lowest)
        if (selectedExtraSpace > 0)
        {
            selectedExtraSpace--;
            extraSpaceVB.setValue(playSettings.getExtraSpace(selectedExtraSpace));
            extraSpaceIdxLbl.setText("(" + std::to_string(selectedExtraSpace + 1) + ')');
        }
    };
    buttons.push_back(&extraSpaceLeftBtn);

    extraSpaceRightBtn = Button(extraSpaceVB.getX() + extraSpaceVB.getWidth() + 10, extraSpaceVB.getY(), 20, 50, ">", textFont, 30);
    extraSpaceRightBtn.onClick = [&]() {
        // Go to extra space setting of a higer dimension (if not higest)
        if (selectedExtraSpace < playSettings.getDimensions() - 1)
        {
            selectedExtraSpace++;
            extraSpaceVB.setValue(playSettings.getExtraSpace(selectedExtraSpace));
            extraSpaceIdxLbl.setText("(" + std::to_string(selectedExtraSpace + 1) + ')');
        }
    };
    buttons.push_back(&extraSpaceRightBtn);

    // Play button, loads the game
    playBtn = Button(860, 780, 200, 80, "Play", textFont, 40);
    playBtn.onClick = [&]() { State::set(std::make_unique<Playing>(playSettings)); };
    buttons.push_back(&playBtn);

    // Exit button, back to main menu
    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = []() { State::set(std::make_unique<Menu>()); };
    buttons.push_back(&exitBtn);
}

/**
 * Updates the pregame settings menu
 * 
 * Shows tooltip when hovered over
 */
void SettingsMenu::update()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    
    for (auto& tooltip : tooltips)
    {
        if (tooltip->pointOnRect(mousePos.x, mousePos.y))
            tooltip->show();
        else
            tooltip->hide();
    }
}

/**
 * Draws everything on the page
 * 
 * @param window the window on which to draw the compontents
 */
void SettingsMenu::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);

    for (auto& label : labels)
        label->draw(window);

    for (auto& valueBox : valueBoxes)
        valueBox->draw(window);

    for (auto& button : buttons)
        button->draw(window);

    for (auto& tooltip : tooltips)
        tooltip->draw(window);
}

/**
 * Handles key presses
 * 
 * @param key key that is pressed
 */
void SettingsMenu::keyPressed(const sf::Keyboard::Key& key)
{
    switch (key)
    {
    case sf::Keyboard::Enter:
        State::set(std::make_unique<Playing>(playSettings));
        break;
    case sf::Keyboard::Escape:
        State::set(std::make_unique<Menu>());
        break;
    case sf::Keyboard::BackSpace:
        for (auto& valueBox : valueBoxes)
        {
            if (valueBox->selected)
            {
                valueBox->removeDigit();
                break;
            }
        }
        break;
    default:
        // Check if a digit has to be added to the currently selected valuebox
        for (auto& valueBox : valueBoxes)
        {
            if (valueBox->selected)
            {
                valueBox->addDigit(key);
                break;
            }
        }
        break;
    }
}

/**
 * Handles key releases
 * 
 * @param key the key that is released
 */
void SettingsMenu::keyReleased(const sf::Keyboard::Key& key)
{

}

/**
 * Handles mouse presses
 * 
 * @param mouse holds the pressed mousebutton and mouse position
 */
void SettingsMenu::mousePressed(const sf::Event::MouseButtonEvent& mouse)
{

}

/**
 * Handles mouse releases
 *
 * @param mouse holds the released mousebutton and mouse position
 */
void SettingsMenu::mouseReleased(const sf::Event::MouseButtonEvent& mouse)
{
    // Check if a valuebox is selected, deselect all other valueboxes
    for (auto& valueBox : valueBoxes)
    {
        if (valueBox->pointOnRect(mouse.x, mouse.y) && !valueBox->selected)
            valueBox->select();
        else
            valueBox->deselect();
    }

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
void SettingsMenu::mouseMoved(const sf::Event::MouseMoveEvent& mouse)
{

}
