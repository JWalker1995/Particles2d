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

        struct Strip
        {
            float heights[2];
            std::vector<std::uint16_t> particles;
        };

        OffsetVector<Strip> strips;
    };

    const RotationCache &get_rotation_cache()
    {
        // Normalize angle inside [0, pi)
        r = fmod(r, M_PI * 2.0f);
        if (r < 0.0f) {r += M_PI * 2.0f;}

        bool flip = r >= M_PI;
        if (flip) {r -= M_PI;}

        unsigned int bin = r * ROTATION_BINS / M_PI_2;
        assert(bin < ROTATION_BINS);

        RotationCache &res = rotation_caches[bin];

        if (res.collision_particles.empty())
        {
            // Rotation cache not initialized yet

            // Make sure there are particles
            assert(!particles.empty());

            // Make sure the particle indexes will fit in a short
            assert(particles.size() <= 0x10000);

            // Calculate min and max rotation of bin
            double rot1 = bin * M_PI_2 / ROTATION_BINS;
            double rot2 = (bin + 1) * M_PI_2 / ROTATION_BINS;

            float sin_rot1 = sin(rot1);
            float cos_rot1 = cos(rot1);
            float sin_rot2 = sin(rot2);
            float cos_rot2 = cos(rot2);

            float slope1 = cos_rot1 / sin_rot1;
            float slope2 = cos_rot2 / sin_rot2;

            // Clear aabb
            res.aabb[Direction::up   ] = 0.0f;
            res.aabb[Direction::down ] = 0.0f;
            res.aabb[Direction::left ] = 0.0f;
            res.aabb[Direction::right] = 0.0f;

            struct RotatedParticle
            {
                float x1;
                float y1;
                float x2;
                float y2;

                /*
                float b1;
                float b2;
                */

                std::uint16_t i;
            };

            WeakSet<RotatedParticle> rotated_particles;

            std::uint16_t i = 0;
            while (i < particles.size())
            {
                // Translate particle
                float px1 = particles[i]->rx * cos_rot1 - particles[i]->ry * sin_rot1;
                float py1 = particles[i]->rx * sin_rot1 + particles[i]->ry * cos_rot1;
                float px2 = particles[i]->rx * cos_rot2 - particles[i]->ry * sin_rot2;
                float py2 = particles[i]->rx * sin_rot2 + particles[i]->ry * cos_rot2;

                RotatedParticle p;
                p.x1 = px1;
                p.y1 = py1;
                p.x2 = px2;
                p.y2 = py2;
                /*
                p.b1 = py - px * slope;
                p.b2 = py + px * slope;
                */
                p.i = i;

                rotated_particles.insert(p);

                // Guarantee px1 <= px2
                if (px1 > px2) {std::swap(px1, px2);}

                // Guarantee py1 <= py2
                if (py1 > py2) {std::swap(py1, py2);}

                // Add particle to aabb
                if (py1 < res.aabb[Direction::up   ]) {res.aabb[Direction::up   ] = py1;}
                if (py2 > res.aabb[Direction::down ]) {res.aabb[Direction::down ] = py2;}
                if (px1 < res.aabb[Direction::left ]) {res.aabb[Direction::left ] = px1;}
                if (px2 > res.aabb[Direction::right]) {res.aabb[Direction::right] = px2;}
                /*
                if (py1 < res.aabb[Direction::up   ]) {res.aabb[Direction::up   ] = py1;}
                //if (py1 > res.aabb[Direction::down ]) {res.aabb[Direction::down ] = py1;}
                if (px1 < res.aabb[Direction::left ]) {res.aabb[Direction::left ] = px1;}
                //if (px1 > res.aabb[Direction::right]) {res.aabb[Direction::right] = px1;}
                //if (py2 < res.aabb[Direction::up   ]) {res.aabb[Direction::up   ] = py2;}
                if (py2 > res.aabb[Direction::down ]) {res.aabb[Direction::down ] = py2;}
                //if (px2 < res.aabb[Direction::left ]) {res.aabb[Direction::left ] = px2;}
                if (px2 > res.aabb[Direction::right]) {res.aabb[Direction::right] = px2;}
                */

                // Insert particle into strip(s)
                signed int s1 = round((px1 - 0.5f) / STRIP_WIDTH);
                signed int s2 = round((px2 + 0.5f) / STRIP_WIDTH);

                while (s1 <= s2)
                {
                    RotationCache::Strip &s = res.strips[s1];
                    if (py1 < s.heights[0]) {s.heights[0] = py1;}
                    if (py2 > s.heights[1]) {s.heights[1] = py2;}
                    s.particles.push_back(i);

                    s1++;
                }

                i++;
            }

            /*
            WeakSet::iterator j = rotated_particles.begin();
            while (j != rotated_particles.end())
            {
                WeakSet::iterator k = rotated_particles.begin();
                while (k != rotated_particles.end())
                {
                    if (j->b1 < k->b1 && j->b2 < k->b2)
                    {
                        break;
                    }
                    k++;
                }

                if (k != rotated_particles.end())
                {
                    rotated_particles.erase(j);
                }
                else
                {
                    j++;
                }
            }

            // There should always be at least one particle that collides
            assert(!rotated_particles.empty());

            res.collision_particles_aabb[Direction::up   ] = 0.0f;
            res.collision_particles_aabb[Direction::down ] = 0.0f;
            res.collision_particles_aabb[Direction::left ] = 0.0f;
            res.collision_particles_aabb[Direction::right] = 0.0f;

            j = rotated_particles.begin();
            while (j != rotated_particles.end())
            {
                res.collision_particles.push_back(j->i);

                if (j->y < res.collision_particles_aabb[Direction::up   ]) {res.collision_particles_aabb[Direction::up   ] = j->y;}
                if (j->y > res.collision_particles_aabb[Direction::down ]) {res.collision_particles_aabb[Direction::down ] = j->y;}
                if (j->x < res.collision_particles_aabb[Direction::left ]) {res.collision_particles_aabb[Direction::left ] = j->x;}
                if (j->x > res.collision_particles_aabb[Direction::right]) {res.collision_particles_aabb[Direction::right] = j->x;}

                j++;
            }
            */
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
