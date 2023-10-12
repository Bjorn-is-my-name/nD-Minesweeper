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

	Button playBtn;					// Button to go to pregame menu
	Button exitBtn;					// Button to exit the game
	std::vector<Button*> buttons;	// Vector for all the buttons on the main menu

public:
	Menu();
	void update() override;
	void draw(sf::RenderWindow& window) override;
	void keyPressed(const sf::Keyboard::Key& key) override;
	void keyReleased(const sf::Keyboard::Key& key) override;
	void mousePressed(const sf::Event::MouseButtonEvent& event) override;
	void mouseReleased(const sf::Event::MouseButtonEvent& event) override;
	void mouseMoved(const sf::Event::MouseMoveEvent& event) override;
};