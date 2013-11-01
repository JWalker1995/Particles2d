#include "world.h"

World::World(int seed, int version)
    : terrain(seed, version)
{
    root = new Chunk();
    root->neighbors[0] = 0;
    root->neighbors[1] = 0;
    root->neighbors[2] = 0;
    root->neighbors[3] = 0;

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

    std::deque<Solid*>::iterator si = solids.begin();
    while (si != solids.end())
    {
        si->vx *= DAMPING;
        si->vy *= DAMPING;
        si->vy += ay;

        si->vr *= ANGULAR_DAMPING;
        si->sin_vr = sin(vr);
        si->cos_vr = cos(vr);

        si++;
    }

    std::deque<Particle*>::iterator pi = particles.begin();
    while (pi != particles.end())
    {
        Solid* solid = si->solid;
        if (solid)
        {
            float dx = si->x - solid->x;
            float dy = si->y - solid->y;

            si->vx = solid->x + solid->vx + solid->cos_vr * dx - solid->sin_vr * dy;
            si->vy = solid->y + solid->vy + solid->sin_vr * dx - solid->cos_vr * dy;

            si->x += si->vx;
            si->y += si->vy;
        }

        pi++;
    }
}

Cell &World::get_cell(signed int x, signed int y)
{
    Cell &cell = cells[x][y];
    if (!cell.state == Cell::t_null)
    {
        terrain.make_cell(x, y, cell);
    }
    return cell;
}

/*
World::Chunk &World::get_bin(signed int cx, signed int cy)
{
    Chunk &bin = bins[cx][cy];
    if (!bin.generated)
    {
        bin.particles = terrain.make_chunk(cx, cy);
        bin.generated = true;
    }
    return bin;
}
*/

/*
Particle *World::get_particle(signed int x, signed int y)
{
    signed int cx = x / CHUNK_SIZE;
    signed int cy = y / CHUNK_SIZE;

    Chunk &bin = get_bin(cx, cy);
    if (bin.particles)
    {
        return bin.particles + x % CHUNK_SIZE + (y % CHUNK_SIZE) * CHUNK_SIZE;
    }
    else
    {
        return 0;
    }
}
*/

World::ProximityTestIterator World::query_particles(float x, float y, float rad)
{
    return World::ProximityTestIterator(this, x, y, rad);
}

World::ProximityTestIterator::ProximityTestIterator(World *world, float x, float y, float rad)
    : world(world)
    , x(x)
    , y(y)
    , rad(rad)
    , bin_x((x - rad) / CHUNK_SIZE)
    , bin_ex((x + rad) / CHUNK_SIZE)
    , bin_sy((y - rad) / CHUNK_SIZE)
    , bin_y(bin_sy)
    , bin_ey((y + rad) / CHUNK_SIZE)
    , bin(world->get_bin(bin_x, bin_y))
    , bin_particles(bin->particles)
    , bin_solids_i(bin->solids.begin())
    , bin_solids_e(bin->solids.end())
    , solid_particle_i(bin_solids_i->particles.begin())
    , solid_particle_e(bin_solids_i->particles.end())
{
    particles_i = static_cast<unsigned int>(-1);

    /*
    unsigned int my = y - rad;


    unsigned int particle_x = sqrt(rad * rad - (my + 1 - y)^2) + x;

    unsigned int particle_sy = my - bin_y * CHUNK_SIZE;
    particles_i = particle_sy * CHUNK_SIZE;
    */
}

Particle *World::ProximityTestIterator::next()
{
    float dx, dy;

    while (true)
    {
        particles_i++;
        if (particles_i >= CHUNK_SIZE * CHUNK_SIZE)
        {
            if (solid_particle_i != solid_particle_e)
            {
                solid_particle_i++;
            }
            if (bin_solids_i != bin_solids_e)
            {
            }
            bin_y++;
            if (bin_y > bin_ey)
            {
                bin_x++;
                if (bin_x > bin_ex) {return 0;}
                bin_y = bin_sy;
            }

            bin_particles = world->get_bin(bin_x, bin_y).particles;
            particles_i = 0;
        }

        dx = bin_x * CHUNK_SIZE + particle_i % CHUNK_SIZE;
        dy = bin_y * CHUNK_SIZE + particle_i / CHUNK_SIZE;
        if (dx * dx + dy * dy < rad * rad && bin_particles[particles_i].type)
        {
            return bin_particles + particle_i;
        }
    }
}
