#ifndef TERRAIN_H
#define TERRAIN_H

#include "world/cell.h"

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

    void make_cell(signed int x, signed int y, Cell &cell);
};

#endif // TERRAIN_H
