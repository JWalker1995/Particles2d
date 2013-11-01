#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

struct ParticleType
{
    ParticleType();

    float mass;

    static const ParticleType *air;
    static const ParticleType *dirt;
};

#endif // PARTICLETYPE_H
