#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstdint>

#include "defs.h"
#include "version.h"

#include "world/world.h"
#include "world/chunk.h"

#include "util/random.h"
#include "terrain/simplexnoise.h"

// DEBUG
#include <iostream>

class Chunk;

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

    void make_chunk(Chunk *chunk);
};

#endif // TERRAIN_H
