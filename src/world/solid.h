#ifndef SOLID_H
#define SOLID_H

#include <cmath>
#include <vector>
#include <set>

#include "defs.h"
#include "world/particle.h"

#include "util/direction.h"
#include "util/offsetvector.h"
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

    struct RotationCache
    {
        float aabb[4];
        std::vector<std::uint16_t> collision_particles;
        float collision_particles_aabb[4];
    };

    const RotationCache &get_rotation_cache()
    {
        // Normalize angle inside [0, 360)
        r = fmod(r, 360.0f);
        if (r < 0.0f) {r += 360.0f;}

        unsigned int bin = r * ROTATION_BINS / M_2_PI;
        RotationCache &res = rotation_caches[bin];

        if (res.collision_particles.empty())
        {
            // Rotation cache not initialized yet

            // Make sure there are particles
            assert(!particles.empty());

            // Make sure the particle indexes will fit in a short
            assert(particles.size() <= 0x10000);

            unsigned int bin = r * ROTATION_BINS / M_2_PI;
            assert(bin < ROTATION_BINS);

            double rot = bin * M_2_PI / ROTATION_BINS + M_PI / ROTATION_BINS;
            // Right now, both the collision_particles and the aabb are only being calculated for this single angle.
            // However, they should be calculated for the range of angles between bin * M_2_PI / ROTATION_BINS and bin * M_2_PI / ROTATION_BINS + M_2_PI / ROTATION_BINS.

            float sin_rot = sin(rot);
            float cos_rot = cos(rot);

            res.aabb[Direction::up   ] = 0.0f;
            res.aabb[Direction::down ] = 0.0f;
            res.aabb[Direction::left ] = 0.0f;
            res.aabb[Direction::right] = 0.0f;

            float slope = cos_rot / sin_rot;

            struct Shadow
            {
                float x;
                float y;

                float b1;
                float b2;

                std::uint16_t i;
            };

            WeakSet<Shadow> rotated_pts;

            std::uint16_t i = 0;
            while (i < particles.size())
            {
                // Translate particle
                float px = particles[i]->rx * cos_rot - particles[i]->ry * sin_rot;
                float py = particles[i]->rx * sin_rot + particles[i]->ry * cos_rot;

                if (py < res.aabb[Direction::up   ]) {res.aabb[Direction::up   ] = py;}
                if (py > res.aabb[Direction::down ]) {res.aabb[Direction::down ] = py;}
                if (px < res.aabb[Direction::left ]) {res.aabb[Direction::left ] = px;}
                if (px > res.aabb[Direction::right]) {res.aabb[Direction::right] = px;}

                Shadow p;
                p.x = px;
                p.y = py;
                p.b1 = py - px * slope;
                p.b2 = py + px * slope;
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

            // There should always be at least one particle that collides
            assert(!rotated_pts.empty());

            res.collision_particles_aabb[Direction::up   ] = 0.0f;
            res.collision_particles_aabb[Direction::down ] = 0.0f;
            res.collision_particles_aabb[Direction::left ] = 0.0f;
            res.collision_particles_aabb[Direction::right] = 0.0f;

            j = rotated_pts.begin();
            while (j != rotated_pts.end())
            {
                res.collision_particles.push_back(j->i);

                if (j->y < res.collision_particles_aabb[Direction::up   ]) {res.collision_particles_aabb[Direction::up   ] = j->y;}
                if (j->y > res.collision_particles_aabb[Direction::down ]) {res.collision_particles_aabb[Direction::down ] = j->y;}
                if (j->x < res.collision_particles_aabb[Direction::left ]) {res.collision_particles_aabb[Direction::left ] = j->x;}
                if (j->x > res.collision_particles_aabb[Direction::right]) {res.collision_particles_aabb[Direction::right] = j->x;}

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

    RotationCache rotation_caches[ROTATION_BINS];
};

#endif // SOLID_H
