#ifndef TERRAIN_H
#define TERRAIN_H

#include "world/particle.h"

#include "util/random.h"
#include "terrain/simplexnoise.h"

#include "defs.h"
#include "version.h"

#include <cstdint>

class Terrain
{
public:
    Terrain(std::uint32_t seed, int version);
    ~Terrain();

    std::uint32_t seed;
    int version;
    std::uint32_t rand;

    struct
    {
        SimplexNoise *main;
    } noise;

    Particle *make_chunk(signed int cx, signed int cy);
};

#endif // TERRAIN_H
