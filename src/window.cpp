#include "window.h"

Window::Window()
{
    Logger::info(std::string("GLFW version: ") + (const char*)glfwGetVersionString());

    glfwSetErrorCallback(window_error_callback);

    // Initialize GLFW
    if (!glfwInit())
    {
        Logger::error("GLFW error!");
        error = true;
        return;
    }

    // Create GLFW window
    // http://www.glfw.org/docs/latest/window.html
    window = glfwCreateWindow(640, 480, "Particles", NULL, NULL);
    if (!window)
    {
        Logger::error("Error: Could not create glfw window!");
        error = true;
        return;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK)
    {
        Logger::error((const char*)glewGetErrorString(glew_init));
        error = true;
        return;
    }

    Logger::info(std::string("GLEW version: ") + (const char*)glewGetString(GLEW_VERSION));

    // Set GLFW options
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    //glfwSetCursorPosCallback(window, cursor_callback);
}

Window::~Window()
{
    if (window) { glfwDestroyWindow(window); }
    glfwTerminate();
}

bool Window::run()
{
    if (error) { return true; }

    state = new StartState();
    state->set_window(this);
    state->init();

    // Wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double sleep_to = glfwGetTime();

    // Main loop
    while (!(glfwWindowShouldClose(window) || error))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // TODO: tick time should adapt based on load
        State *new_state = state->tick(TARGET_SPF);
        if (new_state)
        {
            delete state;
            state = new_state;
            state->set_window(this);
            state->init();
        }

        glFlush();
        glfwSwapBuffers(window);

        sleep_to += TARGET_SPF;
        double time = glfwGetTime();
        if (time < sleep_to)
        {
            // Extra time - sleep
            usleep((sleep_to - time) * 1e6);
        }
        else
        {
            // No extra time
            sleep_to = time;
        }

        glfwPollEvents();
    }

    if (state) { delete state; }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);// GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, or GLFW_CURSOR_DISABLED

    return error;
}

void window_error_callback(int code, const char *str)
{
    switch (code)
    {
    case GLFW_NOT_INITIALIZED:
        Logger::warn("GLFW has not been initialized.");
        break;
    case GLFW_NO_CURRENT_CONTEXT:
        Logger::warn("No context is current for this thread.");
        break;
    case GLFW_INVALID_ENUM:
        Logger::warn("One of the enum parameters for the function was given an invalid enum.");
        break;
    case GLFW_INVALID_VALUE:
        Logger::warn("One of the parameters for the function was given an invalid value.");
        break;
    case GLFW_OUT_OF_MEMORY:
        Logger::warn("A memory allocation failed.");
        break;
    case GLFW_API_UNAVAILABLE:
        Logger::warn("GLFW could not find support for the requested client API on the system.");
        break;
    case GLFW_VERSION_UNAVAILABLE:
        Logger::warn("The requested client API version is not available.");
        break;
    case GLFW_PLATFORM_ERROR:
        Logger::warn("A platform-specific error occurred that does not match any of the more specific categories.");
        break;
    case GLFW_FORMAT_UNAVAILABLE:
        Logger::warn("The clipboard did not contain data in the requested format.");
        break;
    }

    Logger::warn(str);
}
