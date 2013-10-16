#include "world.h"

World::World(int seed, int version)
{
}

void World::tick(float time)
{
    float ay = GRAVITY / time;

    std::vector<DynamicParticle>::iterator i = dynamic_particles.begin();
    while (i != dynamic_particles.end())
    {
        i->vx *= FRICTION;
        i->vy *= FRICTION;
        i->vy += ay;

        i->x += i->vx;
        i->y += i->vy;

        i++;
    }
}
