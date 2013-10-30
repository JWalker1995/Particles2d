#include "world.h"

World::World(int seed, int version)
    : terrain(seed, version)
{
    GLubyte *data = new GLubyte[500 * 500 * 3];
    int i = 0;
    while (i < 500 * 500 * 3)
    {
        data[i] = rand() % 256;
        i++;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
