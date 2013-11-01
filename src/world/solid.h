#ifndef SOLID_H
#define SOLID_H

#include <vector>
#include <set>

struct Solid
{
    Solid()
    {
    }

    float x;
    float y;
    float r;

    float vx;
    float vy;
    float vr;

    float sin_vr;
    float cos_vr;

    float mass;

    void apply_impulse(float x, float y, float ix, float iy)
    {
    }

protected:
    //GLuint texture;
};

#endif // SOLID_H
