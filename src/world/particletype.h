#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

struct ParticleType
{
    ParticleType(const float mass, const unsigned char r, const unsigned char g, const unsigned char b);

    const float mass;

    const unsigned char r;
    const unsigned char g;
    const unsigned char b;

    static const ParticleType *dirt;
    static const ParticleType *rock;
};

#endif // PARTICLETYPE_H
