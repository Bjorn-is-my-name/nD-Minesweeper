#pragma once

#include "GameState.h"
#include "PlaySettings.h"
#include "ValueBox.h"
#include "Button.h"
#include "Label.h"
#include "Tooltip.h"
#include <vector>

class SettingsMenu : public GameState
{
private:
	sf::Font textFont;					// Font for all the displayed text
	PlaySettings playSettings;			// Settings with which to play the game

	Label menutextLbl;					// Menu specific text
	Label dimLbl;						// Label for the dimensions setting
	Label dimSizeLbl;					// Label for the dimension sizes setting
	Label dimSizeIdxLbl;				// Label to indicate the selected dimension size
	Label bombsLbl;						// Label for the bombs setting
	Label extraSpaceLbl;				// Label for the extra space between dimensions setting
	Label extraSpaceIdxLbl;				// Label to indicate the selected extra space between dimension
	std::vector<Label*> labels;			// Vector which holds all the labels on the page

	ValueBox dimVB;						// Valuebox for changing the number of dimensions
	ValueBox dimSizeVB;					// Valuebox for changing a dimension size
	ValueBox bombsVB;					// Valuebox for changing the number of bombs
	ValueBox extraSpaceVB;				// Valuebox for changing the space between a dimension
	std::vector<ValueBox*> valueBoxes;	// Vector which holds all the valueboxes on the page
	
	Button dimSizeLeftBtn;				// Button to go to the dimension size setting 1 dimension lower then the current
	Button dimSizeRightBtn;				// Button to go to the dimension size setting 1 dimension higher then the current
	Button extraSpaceLeftBtn;			// Button to go to the extra space setting 1 dimension lower then the current
	Button extraSpaceRightBtn;			// Button to go to the extra space setting 1 dimension higher then the current
	Button playBtn;						// Button to play the game
	Button exitBtn;						// Button to go back to the main menu
	std::vector<Button*> buttons;		// Vector which holds all the buttons on the page

	ToolTip dimTt;						// Tooltip for the dimensions settings
	ToolTip dimSizeTt;					// Tooltip for the dimension sizes setting
	ToolTip bombsTt;					// Tooltip for the bombs setting
	ToolTip extraSpaceTt;				// Tooltip for the extra space between dimensions setting
	std::vector<ToolTip*> tooltips;		// Vector which holds all the tooltips on the page
	
	int selectedDimSize;				// Current selected dimension size setting
	int selectedExtraSpace;				// Current extra space between dimension setting

public:
	SettingsMenu(const PlaySettings& playSettings = PlaySettings());
	void update() override;
	void draw(sf::RenderWindow& window) override;
	void keyPressed(const sf::Keyboard::Key& key) override;
	void keyReleased(const sf::Keyboard::Key& key) override;
	void mousePressed(const sf::Event::MouseButtonEvent& mouse) override;
	void mouseReleased(const sf::Event::MouseButtonEvent& mouse) override;
	void mouseMoved(const sf::Event::MouseMoveEvent& mouse) override;
};
