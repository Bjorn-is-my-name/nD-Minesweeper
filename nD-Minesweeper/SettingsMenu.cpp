#include "SettingsMenu.h"
#include "Menu.h"
#include "Playing.h"

SettingsMenu::SettingsMenu(const PlaySettings settings)
{
    // Load files, exit game if unsuccesfull
    if (!textFont.loadFromFile("arial.ttf"))
        Game::setState();

    playSettings = settings;

    // Menu text
    menutextLbl = Label(766, 200, "Game Settings", textFont, 60);
    labels.push_back(&menutextLbl);

    // Dimensions input
    dimLbl = Label(600, 408, "Dimensions:", textFont, 30);
    labels.push_back(&dimLbl);

    dimVB = ValueBox(dimLbl.getX() + dimLbl.getWidth() + 20, dimLbl.getY() - dimLbl.getHeight() / 2, 50, 50, playSettings.getDimensions(), 1, 99, textFont, 30);
    dimVB.onDeselect = [&]() {
        int value = dimVB.getValue();
        int min = dimVB.getMin();
        
        if (value < min)
        {
            playSettings.setDimensions(min);
            dimVB.setValue(min);
        }
        else
            playSettings.setDimensions(value);

        bombsVB.setMax(playSettings.getMax());
    };
    valueBoxes.push_back(&dimVB);

    // Dimension sizes input
    dimSizeLbl = Label(600, 609, "Size:", textFont, 30);
    labels.push_back(&dimSizeLbl);

    dimSizeIdxLbl = Label(dimSizeLbl.getX() + dimSizeLbl.getWidth() + 62, dimSizeLbl.getY() - dimSizeLbl.getHeight() / 2 - 30, "(1)", textFont, 18);
    labels.push_back(&dimSizeIdxLbl);

    dimSizeVB = ValueBox(dimSizeLbl.getX() + dimSizeLbl.getWidth() + 50, dimSizeLbl.getY() - dimSizeLbl.getHeight() / 2, 50, 50, playSettings.getDimensionSize(selectedDimSize), 1, 99, textFont, 30);
    dimSizeVB.onDeselect = [&]() {
        int value = dimSizeVB.getValue();
        int min = dimSizeVB.getMin();

        if (value < min)
        {
            playSettings.setDimensionSize(selectedDimSize, min);
            dimSizeVB.setValue(min);
        }
        else
            playSettings.setDimensionSize(selectedDimSize, value);
        
        bombsVB.setMax(playSettings.getMax());
    };
    valueBoxes.push_back(&dimSizeVB);

    dimSizeLeftBtn = Button(dimSizeVB.getX() - 30, dimSizeVB.getY(), 20, 50, "<", textFont, 30);
    dimSizeLeftBtn.onClick = [&]() {
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
        if (selectedDimSize < playSettings.getDimensions() - 1)
        {
            selectedDimSize++;
            dimSizeVB.setValue(playSettings.getDimensionSize(selectedDimSize));
            dimSizeIdxLbl.setText("(" + std::to_string(selectedDimSize + 1) + ')');
        }
    };
    buttons.push_back(&dimSizeRightBtn);

    // Bombs input
    bombsLbl = Label(1000, 408, "Bombs:", textFont, 30);
    labels.push_back(&bombsLbl);

    bombsVB = ValueBox(bombsLbl.getX() + bombsLbl.getWidth() + 20, bombsLbl.getY() - bombsLbl.getHeight() / 2, 50, 50, playSettings.getBombs(), 0, playSettings.getMax(), textFont, 30);
    bombsVB.onDeselect = [&]() {
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

    // Extra space input
    extraSpaceLbl = Label(1000, 609, "Size:", textFont, 30);
    labels.push_back(&extraSpaceLbl);

    extraSpaceIdxLbl = Label(extraSpaceLbl.getX() + extraSpaceLbl.getWidth() + 62, extraSpaceLbl.getY() - extraSpaceLbl.getHeight() / 2 - 30, "(1)", textFont, 18);
    labels.push_back(&extraSpaceIdxLbl);

    extraSpaceVB = ValueBox(extraSpaceLbl.getX() + extraSpaceLbl.getWidth() + 50, extraSpaceLbl.getY() - extraSpaceLbl.getHeight() / 2, 50, 50, playSettings.getExtraSpace(selectedExtraSpace), 0, 99, textFont, 30);
    extraSpaceVB.onDeselect = [&]() {
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
        if (selectedExtraSpace < playSettings.getDimensions() - 1)
        {
            selectedExtraSpace++;
            extraSpaceVB.setValue(playSettings.getExtraSpace(selectedExtraSpace));
            extraSpaceIdxLbl.setText("(" + std::to_string(selectedExtraSpace + 1) + ')');
        }
    };
    buttons.push_back(&extraSpaceRightBtn);

    // Play button
    playBtn = Button(860, 780, 200, 80, "Play", textFont, 40);
    playBtn.onClick = [&]() { Game::setState(new Playing(playSettings)); };
    buttons.push_back(&playBtn);

    // Exit button
    exitBtn = Button(1860, 20, 40, 40, "x", textFont, 30);
    exitBtn.onClick = []() { Game::setState(new Menu()); };
    buttons.push_back(&exitBtn);
}

void SettingsMenu::update()
{

}

void SettingsMenu::draw(sf::RenderWindow& window)
{
    for (auto& label : labels)
        label->draw(window);

    for (auto& valueBox : valueBoxes)
        valueBox->draw(window);

    for (auto& button : buttons)
        button->draw(window);
}

void SettingsMenu::keyPressed(const sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Enter:
        Game::setState(new Playing(playSettings));
        break;
    case sf::Keyboard::Escape:
        Game::setState(new Menu());
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

void SettingsMenu::mousePressed(const sf::Event::MouseButtonEvent event)
{

}

void SettingsMenu::mouseReleased(const sf::Event::MouseButtonEvent event)
{
    for (auto& valueBox : valueBoxes)
    {
        if (valueBox->pointOnRect(event.x, event.y) && !valueBox->selected)
            valueBox->select();
        else
            valueBox->deselect();
    }

    for (auto& button : buttons)
    {
        if (button->pointOnRect(event.x, event.y))
        {
            button->onClick();
            break;
        }
    }
}