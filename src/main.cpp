#include <iostream>

#include "util/logger.h"
#include "window.h"

int main(int argc, char** argv)
{
    Logger::info("Starting...");

    Window window;
    bool res = window.run();

    Logger::info("Ending...");

    return res;
}

