#ifndef PARTICLE_H
#define PARTICLE_H

#include "world/solid.h"
#include "world/particletype.h"

struct Particle
{
    float rx;
    float ry;

    float vx;
    float vy;

    const ParticleType *type;

    static ParticleType types[2];
};

#endif // PARTICLE_H
