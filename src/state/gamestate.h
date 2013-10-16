#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "state/state.h"
#include "world/world.h"

class GameState : public State
{
public:
    GameState(int seed);
    ~GameState();

    virtual void init();
    virtual State *tick(float time);

protected:
    World *world;
};

#endif // GAMESTATE_H
