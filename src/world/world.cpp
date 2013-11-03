#include "world.h"

World::World(int seed, int version)
    : terrain(new Terrain(seed, version))
{
    root = new Chunk(this, 0, 0);
    root->neighbors[0] = 0;
    root->neighbors[1] = 0;
    root->neighbors[2] = 0;
    root->neighbors[3] = 0;

    Particle *p = new Particle();

    p->x = 32.0f;
    p->y = 16.0f;
    p->vx = 0.0f;
    p->vy = 0.0f;

    p->solid = 0;
    p->type = ParticleType::rock;

    root->particles.insert(p);
    active_chunks.insert(root);
}

void World::tick(float time)
{
    float ay = GRAVITY * time;

    WeakSet<Solid*>::iterator si = solids.begin();
    while (si != solids.end())
    {
        Solid *s = *si;
        s->vx *= DAMPING;
        s->vy *= DAMPING;
        s->vy += ay;

        s->vr *= ANGULAR_DAMPING;
        s->sin_vr = sin(s->vr);
        s->cos_vr = cos(s->vr);

        si++;
    }

    WeakSet<Chunk*>::iterator ci = active_chunks.begin();
    while (ci != active_chunks.end())
    {
        Chunk* c = *ci;

        if (c->particles.empty())
        {
            ci = active_chunks.erase(ci);
            continue;
        }

        WeakSet<Particle*>::iterator pi = c->particles.begin();
        WeakSet<Particle*>::iterator pie = c->particles.end();
        while (pi != pie)
        {
            Particle *p = *pi;

            Solid* solid = p->solid;
            if (solid)
            {
                // Get velocity from solid
                float dx = p->x - solid->x;
                float dy = p->y - solid->y;

                p->vx = solid->vx - solid->sin_vr * dx + solid->cos_vr * dy;
                p->vy = solid->vy - solid->cos_vr * dx + solid->sin_vr * dy;
            }
            else
            {
                p->vx *= DAMPING;
                p->vy *= DAMPING;
                p->vy += ay;
            }

            signed int old_x = p->x;
            signed int old_y = p->y;

            p->x += p->vx;
            p->y += p->vy;

            signed int new_x = p->x;
            signed int new_y = p->y;

            if (old_x != new_x || old_y != new_y)
            {
                Chunk *nc = c;
                if (new_x < 0) {nc = nc->neighbor(this, Chunk::neighbor_left);}
                else if (new_x > CHUNK_SIZE) {nc = nc->neighbor(this, Chunk::neighbor_right);}
                if (new_y < 0) {nc = nc->neighbor(this, Chunk::neighbor_up);}
                else if (new_y > CHUNK_SIZE) {nc = nc->neighbor(this, Chunk::neighbor_down);}

                Cell *cl = nc->cells + old_x + old_y * CHUNK_SIZE;
                Cell *ncl = nc->cells + new_x + new_y * CHUNK_SIZE;
                if (ncl->state == Cell::t_air)
                {
                    view.update_pixel(c->x + old_x, c->y + old_y, 0, 0, 0);
                    view.update_pixel(c->x + new_x, c->y + new_y, p->type->r, p->type->g, p->type->b);

                    cl->state = Cell::t_air;
                    ncl->state = Cell::t_dynamic;
                    ncl->particle = p;
                }
                else if (ncl->state == Cell::t_static)
                {
                    std::cout << "collision" << std::endl;
                }

                if (nc != c)
                {
                    if (nc->particles.empty())
                    {
                        active_chunks.insert(nc);
                    }

                    pi = c->particles.erase(pi);
                    nc->particles.insert(p);

                    if (c->particles.empty())
                    {
                        ci = active_chunks.erase(ci);
                    }
                }
            }

            pi++;
        }

        ci++;
    }
}

void World::draw()
{
    view.draw();
}

void World::initialize_chunk(Chunk *chunk)
{
    terrain->make_chunk(chunk);

    Cell *cell = chunk->cells;
    Cell *cell_e = chunk->cells + CHUNK_SIZE * CHUNK_SIZE;

    signed int x = chunk->x;
    signed int y = chunk->y;

    while (cell < cell_e)
    {
        if (cell->state == Cell::t_static)
        {
            view.update_pixel(x, y, cell->type->r, cell->type->g, cell->type->b);
        }

        x++;
        if (x % CHUNK_SIZE == 0)
        {
            x = 0;
            y++;
        }

        cell++;
    }
}

/*
Cell &World::get_cell(signed int x, signed int y)
{
    Cell &cell = cells[x][y];
    if (!cell.state == Cell::t_null)
    {
        terrain.make_cell(x, y, cell);
    }
    return cell;
}
*/

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

/*
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
*/
