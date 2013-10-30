#ifndef PARTICLE_H
#define PARTICLE_H

#include "world/solid.h"

struct Particle
{
    Solid *solid;

    char type;

    //static void register_type(ParticleType type);
};

#endif // PARTICLE_H
