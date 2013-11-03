#include "particletype.h"

const ParticleType *ParticleType::dirt = new ParticleType(1.0f, 87, 59, 12);
const ParticleType *ParticleType::rock = new ParticleType(2.0f, 64, 64, 64);

ParticleType::ParticleType(const float mass, const unsigned char r, const unsigned char g, const unsigned char b)
    : mass(mass)
    , r(r)
    , g(g)
    , b(b)
{}
