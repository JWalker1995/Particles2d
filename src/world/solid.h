#ifndef SOLID_H
#define SOLID_H

#include <vector>

#include "world/particle.h"

struct Solid
{
    float x;
    float y;
    float r;

    float vx;
    float vy;
    float vr;

    float mass;

    struct SolidParticle
    {
        float x;
        float y;

        char type;
    };

    std::vector<SolidParticle> particles;

    void update_particles()
    {
        float dx = x;
        float dy = y;

        x = 0.0f;
        y = 0.0f;
        mass = 0.0f;

        std::vector<SolidParticle>::iterator i = particles.begin();
        while (i != particles.end())
        {
            float m = 1.0f;

            x += i->x * m;
            y += i->y * m;
            mass += m;

            i++;
        }

        x /= mass;
        y /= mass;

        dx -= x;
        dy -= y;

        i = particles.begin();
        while (i != particles.end())
        {
            i->x += dx;
            i->y += dy;
            i++;
        }
    }

    void apply_impulse(float x, float y, float ix, float iy)
    {
    }
};

#endif // SOLID_H
