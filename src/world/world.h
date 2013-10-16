#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "defs.h"
#include "util/offsetvector.h"

class World
{
public:
    World(int seed, int version);

    void tick(float time);

protected:

    struct StaticParticle
    {
        char type;
    };
    struct DynamicParticle
    {
        char type;
        float x;
        float y;
        float vx;
        float vy;
    };

    struct Bin
    {
        Bin() {}

        StaticParticle *static_particles = 0;
    };

    OffsetVector<OffsetVector<Bin>> bins;

    std::vector<DynamicParticle> dynamic_particles;

    /*
    Particle queries:
        Get particles touching point (max 9 results)
        Get all magnetic particles within rad
        Get all particles touching line segment
    */
};

#endif // WORLD_H
