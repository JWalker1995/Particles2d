#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

class Random : public std::mt19937_64
{
public:
    Random()
        : Random(get_timestamp_seed())
    {}

    Random(unsigned int seed)
        : mersenne_twister_engine(seed)
    {}

    static unsigned int get_timestamp_seed()
    {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }
};

#endif // RANDOM_H
