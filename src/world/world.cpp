#include "world.h"

World::World(int seed, int version)
    : terrain(seed, version)
{
}

void World::tick(float time)
{
    float ay = GRAVITY / time;

    std::vector<Solid>::iterator i = solids.begin();
    while (i != solids.end())
    {
        i->vx *= FRICTION;
        i->vy *= FRICTION;
        i->vy += ay;

        int orig_x = i->x;
        int orig_y = i->y;
        
        i->x += i->vx;
        i->y += i->vy;
        
        int new_x = i->x;
        int new_y = i->y;
        
        if (orig_x != new_x || orig_y != new_y)
        {
            Particle &particle = get_particle(new_x, new_y);
            if (particle.type)
            {

            }
        }

        i++;
    }

    // Color stack - not good for many particles in a small space
    // Last color
    //     When pixel moves over
}

Particle &World::get_particle(signed int x, signed int y)
{
    signed int cx = x / CHUNK_SIZE;
    signed int cy = y / CHUNK_SIZE;

    Particle *&particles = bins[cx][cy].particles;
    if (!particles) {particles = terrain.make_chunk(cx, cy);}
    return particles[x % CHUNK_SIZE + (y % CHUNK_SIZE) * CHUNK_SIZE];
}
