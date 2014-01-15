#ifndef SOLID_H
#define SOLID_H

#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

#include "defs.h"

#include "object.h"
#include "particle.h"

#include "util/direction.h"
#include "util/offsetvector.h"
#include "util/weakset.h"

struct Solid : public Object
{
    Solid()
    {
    }

    float r;

    float vx;
    float vy;
    float vr;

    float sin_vr;
    float cos_vr;

    //Chunk *chunk;

    std::vector<Particle> particles;

    struct RotationCache
    {
        float aabb[4];
        std::vector<std::uint16_t> collision_particles;
        float collision_particles_aabb[4];

        struct Stripe
        {
            float height;
            float tree_height;

            std::vector<std::uint16_t> particles;
        };

        signed int stripe_offset;
        std::vector<Stripe> stripes;
    };

    struct Sector
    {
        float rad;
        float tree_rad;

        std::vector<std::uint16_t> particles;
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
            double rot = bin * M_PI_2 / ROTATION_BINS;

            float sin_rot = sin(rot);
            float cos_rot = cos(rot);

            float slope = cos_rot / sin_rot;

            // Clear aabb
            res.aabb[Direction::up   ] = 0.0f;
            res.aabb[Direction::down ] = 0.0f;
            res.aabb[Direction::left ] = 0.0f;
            res.aabb[Direction::right] = 0.0f;

            /*
            struct RotatedParticle
            {
                float x;
                float y;

                //float b1;
                //float b2;

                std::uint16_t i;
            };

            WeakSet<RotatedParticle> rotated_particles;
            */

            struct RotatedParticle
            {
                float x;
                float y;
            };
            RotatedParticle *rotated_particles = new RotatedParticle[particles.size()];

            std::uint16_t i = 0;
            while (i < particles.size())
            {
                // Translate particle
                float px = particles[i]->rx * cos_rot - particles[i]->ry * sin_rot;
                float py = particles[i]->rx * sin_rot + particles[i]->ry * cos_rot;

                RotatedParticle p;
                p.x = px;
                p.y = py;
                /*
                p.b1 = py - px * slope;
                p.b2 = py + px * slope;
                p.i = i;
                */

                rotated_particles.insert(p);

                // Add particle to aabb
                if (py < res.aabb[Direction::up   ]) {res.aabb[Direction::up   ] = py;}
                if (py > res.aabb[Direction::down ]) {res.aabb[Direction::down ] = py;}
                if (px < res.aabb[Direction::left ]) {res.aabb[Direction::left ] = px;}
                if (px > res.aabb[Direction::right]) {res.aabb[Direction::right] = px;}

                i++;
            }

            // Expand aabb by the particle radius
            res.aabb[Direction::up   ] -= 0.5f;
            res.aabb[Direction::down ] += 0.5f;
            res.aabb[Direction::left ] -= 0.5f;
            res.aabb[Direction::right] += 0.5f;

            // Initialize the stripes
            res.stripe_offset = floor(res.aabb[Direction::left] / STRIPE_WIDTH);
            signed int stripe_max = ceil(res.aabb[Direction::right] / STRIPE_WIDTH);
            res.stripes.resize(stripe_max - res.stripe_offset);

            // Add particles into stripes
            i = 0;
            while (i < particles.size())
            {
                RotatedParticle &p = rotated_particles[i];

                // Insert particle into stripe(s)
                signed int s1 = round((p.x - 0.5f) / STRIPE_WIDTH);
                signed int s2 = round((p.x + 0.5f) / STRIPE_WIDTH);

                while (s1 <= s2)
                {
                    RotationCache::Stripe &s = res.stripes[s1 - res.stripe_offset];
                    if (p.y < s.heights[0]) {s.heights[0] = p.y;}
                    if (p.y > s.heights[1]) {s.heights[1] = p.y;}

                    // TODO: Only push boundary particles (called collision particles above).
                    s.particles.push_back(i);

                    s1++;
                }

                i++;
            }

            bool compare_particle(std::uint16_t a, std::uint16_t b)
            {
                return rotated_particles[a].y < rotated_particles[b].y;
            }

            // TODO: Possible optimization - move sort above the particle push, so particles are pushed in the correct order.
            std::vector<RotationCache::Stripe>::iterator si = res.stripes.begin();
            while (si != res.stripes.end())
            {
                std::sort(si->particles.begin(), si->particles.end(), compare_particle);

                si++;
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

    static void collide(Solid *s1, Solid *s2)
    {
        // TODO: Possible optimization: change int to char

        bool collides(unsigned int sec1, unsigned int arc1, unsigned int sec2, unsigned int arc2)
        {
            return false;
        };

        void collide_sectors(unsigned int sec1, unsigned int sec2, unsigned int arc)
        {
            if (arc)
            {
                if (collides(sec1, arc, sec2, arc))
                {
                    if (collides(sec1, arc, sec2, arc / 2))
                    {
                        collide_sectors()
                    }
                }
            }
            else
            {
                s1->sectors[sec1]
            }
        };

        unsigned int cont = 0;

        unsigned int sector[2] = {0};
        unsigned int arc = SOLID_SECTORS;

        bool cur_s = 0;

        while (true)
        {
            if (!collides(sector[cur_s], arc, sector[!cur_s], cur_s ? arc / 2 : arc))
            {
                sector[cur_s] += arc / 2;
            }

            /*

            If 0:8 & 0:8 Then
                If 0:8 & 0:4 Then
                    Try 0:4 & 0:4
                    If 0:8 & 4:8 Then
                        Try 0:4 & 4:8
                    End
                Else
                    Try 0:4 & 4:8
                End
            End
            If 8:16 & 0:4 Then
                Try 8:12 & 0:4
            End
            Try 8:12 & 4:8

            */

            if (collides(sector1, arc, sector2, arc))
            {
                if (collides(sector1, arc, sector2, arc / 2))
                {
                    arc /= 2;
                    continue;
                }
                else
                {
                    sector2 += arc / 2;
                    arc /= 2;
                    continue;
                }
            }
            else
            {
                sector1 += arc;
                if (sector1 >= SOLID_SECTORS) {break;}

                if (collides(sector1, arc / 2, sector2, arc))
                {
                    arc /= 2;
                    continue;
                }
                else
                {
                    sector1 += arc / 2;
                    arc /= 2;
                    continue;
                }
            }
        }


        // Calculate angle from s1 to s2
        // TODO: Optimize out atan2
        float r = atan2(s2->x - s1->x, s2->y - s1->y);

        float r1 = r - s1->r;
        float r2 = r - s2->r + 180.0f;

        // Normalize angles inside [0, pi * 2)
        r1 = fmod(r1, M_PI * 2.0f);
        if (r1 < 0.0f) {r1 += M_PI * 2.0f;}

        r2 = fmod(r2, M_PI * 2.0f);
        if (r2 < 0.0f) {r2 += M_PI * 2.0f;}

        unsigned int bin1 = round(r1 * SOLID_SECTORS / M_PI_2);
        assert(bin1 < SOLID_SECTORS);

        unsigned int bin2 = round(r2 * SOLID_SECTORS / M_PI_2);
        assert(bin2 < SOLID_SECTORS);

        Sector &sector1 = s1->sectors[bin1];
        Sector &sector2 = s2->sectors[bin2];
    }

    void apply_impulse(float x, float y, float ix, float iy)
    {
    }

protected:
    //GLuint texture;

    RotationCache rotation_caches[ROTATION_BINS];

    Sector sectors[SOLID_SECTORS];
};

#endif // SOLID_H
