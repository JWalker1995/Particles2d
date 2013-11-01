#ifndef PARTICLE_H
#define PARTICLE_H

#include "world/solid.h"
#include "world/particletype.h"

struct Particle
{
    float x;
    float y;

    float vx;
    float vy;

    Solid *solid = 0;
    float d;

    char type;

    static ParticleType types[2];
};

#endif // PARTICLE_H
