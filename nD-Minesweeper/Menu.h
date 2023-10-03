#pragma once

#include "GameState.h"
#include <vector>
#include "Label.h"
#include "Button.h"

class Menu : public GameState
{
private:
	sf::Font textFont;
	Label menutextLbl;
	Button playBtn;
	Button exitBtn;
	std::vector<Button*> buttons;

public:
	Menu();
	void update() override;
	void draw(sf::RenderWindow& window) override;
	void keyPressed(const sf::Keyboard::Key key) override;
	void mousePressed(const sf::Event::MouseButtonEvent event) override;
	void mouseReleased(const sf::Event::MouseButtonEvent event) override;
};