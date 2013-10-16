#ifndef LOGGER_H
#define LOGGER_H

//#include <cstdlib>
#include <iostream>
#include <string>

class Logger
{
public:
    Logger();

    static void info(std::string msg)
	{
		std::cout << msg.data() << std::endl;
	}
    static void warn(std::string msg)
	{
		std::cerr << "Warning: " << msg.data() << std::endl;
	}
    static void error(std::string msg)
	{
		std::cerr << "Error: " << msg.data() << std::endl;
	}
};

#endif // LOGGER_H
