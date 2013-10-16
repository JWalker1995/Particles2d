#ifndef STARTSTATE_H
#define STARTSTATE_H

#include "state/state.h"

class StartState : public State
{
public:
    StartState();

protected:
    virtual State *tick(float time);
};

#endif // STARTSTATE_H
