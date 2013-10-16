#include "startstate.h"

StartState::StartState()
{
}

State *StartState::tick(float time)
{
    // Immediately go to the game state
    return new GameState(123);
}
