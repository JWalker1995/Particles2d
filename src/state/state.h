#ifndef STATE_H
#define STATE_H

#include "gl.h"
#include "window.h"

#include "version.h"

// States included at bottom

class State
{
    friend class Window;

public:
    State() {}
    virtual ~State() {}

    virtual void init() {}
    virtual State *tick(float time) = 0;

protected:
    Window *get_window() { return window; }
    GLFWwindow *get_glfw_window() { return window->get_glfw_window(); }

private:
    Window *window;

    void set_window(Window *window) { this->window = window; }
};

#include "startstate.h"
#include "gamestate.h"

#endif // STATE_H
