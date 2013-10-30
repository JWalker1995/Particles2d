#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "defs.h"

#include "world/particle.h"
#include "terrain/terrain.h"
#include "util/offsetvector.h"

class World
{
public:
    World(int seed, int version);

    void tick(float time);

protected:
    Particle &get_particle(signed int x, signed int y);

    struct Bin
    {
        Bin() {}

        Particle *particles = 0;
    };

    OffsetVector<OffsetVector<Bin>> bins;

    std::vector<Solid> solids;

    Terrain terrain;

    /*
    Particle queries:
        Get particles touching point (max 9 results)
        Get all magnetic particles within rad
        Get all particles touching line segment
    */
};

#endif // WORLD_H
