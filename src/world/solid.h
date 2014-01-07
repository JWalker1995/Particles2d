#ifndef SOLID_H
#define SOLID_H

#include <cmath>
#include <vector>
#include <set>

#include "defs.h"
#include "world/particle.h"

#include "util/weakset.h"

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

    std::vector<Particle> particles;

    const std::vector<std::uint16_t> &get_collision_particles(float rotation)
    {
        unsigned int bin = rotation * ROTATION_BINS / M_2_PI;
        std::vector<std::uint16_t> &res = collision_particles[bin];

        if (res.empty())
        {
            assert(particles.size() <= 0x10000);

            struct Point
            {
                float x;
                float y;
                std::uint16_t i;
            };

            WeakSet<Point> rotated_pts;

            double rot = bin * M_2_PI / ROTATION_BINS;

            float sin_rot = sin(rotation);
            float cos_rot = cos(rotation);

            float slope = cos_rot / sin_rot;

            std::uint16_t i = 0;
            while (i < particles.size())
            {
                Point p;
                p.x = particles[i]->x * cos_rot - particles[i]->y * sin_rot;
                p.y = particles[i]->x * sin_rot + particles[i]->y * cos_rot;
                p.i = i;

                rotated_pts.insert(p);
                i++;
            }

            // Could use optimization
            WeakSet::iterator j = rotated_pts.begin();
            while (j != rotated_pts.end())
            {
                bool inc = true;

                WeakSet::iterator k = rotated_pts.begin();
                while (k != rotated_pts.end())
                {
                    float b1 = k->y + k->x * slope;
                    float b2 = k->y - k->x * slope;
                    if (j->y < b1 - slope * j->x && j->y < b2 + slope * j->x)
                    {
                        rotated_pts.erase(j);
                        inc = false;
                        break;
                    }
                    k++;
                }
                j += inc;
            }
        }
        return res;
    }

    void apply_impulse(float x, float y, float ix, float iy)
    {
    }

protected:
    //GLuint texture;

    std::vector<std::uint16_t> collision_particles[ROTATION_BINS];
};

#endif // SOLID_H
