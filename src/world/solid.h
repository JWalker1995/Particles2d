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

    float max_rad;

    Chunk *chunk;

    std::vector<Particle> particles;

    const std::vector<std::uint16_t> &get_collision_particles()
    {
        unsigned int bin = r * ROTATION_BINS / M_2_PI;
        std::vector<std::uint16_t> &res = collision_particles[bin];

        if (res.empty())
        {
            assert(particles.size() <= 0x10000);

            unsigned int bin = r * ROTATION_BINS / M_2_PI;
            assert(bin < ROTATION_BINS);

            double rot = min_ang + M_PI / ROTATION_BINS;

            float sin_rot = sin(rot);
            float cos_rot = cos(rot);

            float slope = cos_rot / sin_rot;

            struct Shadow
            {
                float b1;
                float b2;

                std::uint16_t i;
            };

            WeakSet<Shadow> rotated_pts;

            std::uint16_t i = 0;
            while (i < particles.size())
            {
                float x = particles[i]->rx * cos_rot - particles[i]->ry * sin_rot;
                float y = particles[i]->rx * sin_rot + particles[i]->ry * cos_rot;

                Shadow p;
                p.b1 = y - x * slope;
                p.b2 = y + x * slope;
                p.i = i;

                rotated_pts.insert(p);
                i++;
            }

            WeakSet::iterator j = rotated_pts.begin();
            while (j != rotated_pts.end())
            {
                WeakSet::iterator k = rotated_pts.begin();
                while (k != rotated_pts.end())
                {
                    if (j->b1 < k->b1 && j->b2 < k->b2)
                    {
                        break;
                    }
                    k++;
                }

                if (k != rotated_pts.end())
                {
                    rotated_pts.erase(j);
                }
                else
                {
                    j++;
                }
            }

            j = rotated_pts.begin();
            while (j != rotated_pts.end())
            {
                res.push_back(j->i);
                j++;
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
