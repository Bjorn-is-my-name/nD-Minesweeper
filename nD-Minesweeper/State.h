#pragma once

#include <memory>
#include "GameState.h"

/**
 * State singleton to hold the gamestate
 */
class State
{
private:
	State() {}

public:
	static GameState& get();
	static void set(std::unique_ptr<GameState>&& newState);
	static void clear();
	static bool isSet();
};

