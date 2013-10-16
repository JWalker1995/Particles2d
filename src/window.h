// state.h has to be included before the window definiton
#include "state/state.h"

#ifndef WINDOW_H
#define WINDOW_H

#include <unistd.h>

#include <string>

#include "gl.h"

#include "defs.h"
#include "util/logger.h"

class State;

class Window
{
public:
    Window();
    ~Window();

    bool run();

    GLFWwindow *get_glfw_window() { return window; }
    int get_width() { return width; }
    int get_height() { return height; }

protected:
    State *state;
	
    bool error = false;

    GLFWwindow *window = 0;

private:
    int width;
    int height;
};

void window_error_callback(int code, const char *str);

#endif // WINDOW_H
