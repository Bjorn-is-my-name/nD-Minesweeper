#include "State.h"
#include <iostream>
static std::unique_ptr<GameState> state;

/**
 * Returns the state
 * 
 * @return the gamestate
 */
GameState& State::get()
{
	return *state;
}

/**
 * Sets the gamestate
 * 
 * @param newState the new gamestate
 */
void State::set(std::unique_ptr<GameState>&& newState)
{
	if (newState)
		state = std::move(newState);
	else
		clear();
}


/**
 * Clears the gamestate
 */
void State::clear()
{
	state.reset(nullptr);
}

/**
 * Checks if the gamestate is set
 * 
 * @return 'true' if set, else 'false'
 */
bool State::isSet()
{
	return state != nullptr;
}
