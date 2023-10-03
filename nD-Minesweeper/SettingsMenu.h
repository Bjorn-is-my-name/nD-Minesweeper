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
	sf::Font textFont;
	PlaySettings playSettings;

	Label menutextLbl;
	Label dimLbl;
	Label dimSizeLbl;
	Label dimSizeIdxLbl;
	Label extraSpaceLbl;
	Label extraSpaceIdxLbl;
	Label bombsLbl;
	std::vector<Label*> labels;

	ValueBox dimVB;
	ValueBox dimSizeVB;
	ValueBox extraSpaceVB;
	ValueBox bombsVB;
	std::vector<ValueBox*> valueBoxes;
	
	Button dimSizeLeftBtn;
	Button dimSizeRightBtn;
	Button extraSpaceLeftBtn;
	Button extraSpaceRightBtn;
	Button playBtn;
	Button exitBtn;
	std::vector<Button*> buttons;

	ToolTip dimTt;
	ToolTip dimSizeTt;
	ToolTip bombsTt;
	ToolTip extraSpaceTt;
	std::vector<ToolTip*> tooltips;
	
	int selectedDimSize;
	int selectedExtraSpace;

public:
	SettingsMenu(const PlaySettings playSettings = PlaySettings());
	void update() override;
	void draw(sf::RenderWindow& window) override;
	void keyPressed(const sf::Keyboard::Key key) override;
	void mousePressed(const sf::Event::MouseButtonEvent event) override;
	void mouseReleased(const sf::Event::MouseButtonEvent event) override;
};
