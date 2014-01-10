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
    float damping = pow(DAMPING, time);
    float angular_damping = pow(ANGULAR_DAMPING, time);

    WeakSet<Solid*>::iterator si = solids.begin();
    WeakSet<Solid*>::iterator sie = solids.end();
    while (si != sie)
    {
        Solid *s = *si;

        // TODO: Remove sqrt
        float steps = floor(s->max_rad * s->vr + sqrt(s->vx * s->vx + s->vy * s->vy));
        
        s->vx *= damping;
        s->vy *= damping;
        s->vr *= angular_damping;

        s->vy += ay;

        float vx = s->vx / steps;
        float vy = s->vy / steps;
        float vr = s->vr / steps;

        unsigned int i = steps;
        while (i--)
        {
            s->x += vx;
            s->y += vy;
            s->r += vr;

            float sin_r = sin(s->r);
            float cos_r = cos(s->r);

            Solid::RotationCache rc = s->get_rotation_cache();

            signed int aabb[4];
            aabb[Direction::up   ] = s->y + rc.aabb[Direction::up   ];
            aabb[Direction::down ] = s->y + rc.aabb[Direction::down ];
            aabb[Direction::left ] = s->x + rc.aabb[Direction::left ];
            aabb[Direction::right] = s->x + rc.aabb[Direction::right];

            Chunk *chunk = s->chunk;

            const std::vector<std::uint16_t> &cps = s->get_collision_particles();
            std::vector<std::uint16_t>::const_iterator cpsi = cps.cbegin();
            while (cpsi != cps.cend())
            {
                unsigned int i = *cpsi;

                signed int x = s->x + s->particles[i]->rx * cos_r - s->particles[i]->ry * sin_r;
                signed int y = s->y + s->particles[i]->rx * sin_r + s->particles[i]->ry * cos_r;

                signed int cx = (x / CHUNK_SIZE) * CHUNK_SIZE;
                signed int cy = (y / CHUNK_SIZE) * CHUNK_SIZE;

                if (cx != chunk->x || cy != chunk->y)
                {
                    while (cx < chunk->x)
                    {
                        chunk = chunk->neighbor(this, Chunk::neighbor_left);
                        cx += CHUNK_SIZE;
                    }
                    while (cx > chunk->x)
                    {
                        chunk = chunk->neighbor(this, Chunk::neighbor_right);
                        cx -= CHUNK_SIZE;
                    }
                    while (cy < chunk->y)
                    {
                        chunk = chunk->neighbor(this, Chunk::neighbor_up);
                        cy += CHUNK_SIZE;
                    }
                    while (cx > chunk->x)
                    {
                        chunk = chunk->neighbor(this, Chunk::neighbor_down);
                        cy -= CHUNK_SIZE;
                    }
                }

                Cell *cell = chunk->cells + (x - cx) + (y - cy) * CHUNK_SIZE;

                switch (cell->state)
                {
                case Cell::state_air:
                    break;
                case Cell::state_static:
                    break;
                case Cell::state_solid:
                    break;
                case Cell::state_particle:
                    break;
                }

                cpsi++;
            }
        }

        si++;
    }

    WeakSet<Solid*>::iterator si = solids.begin();
    WeakSet<Solid*>::iterator sie = solids.end();
    while (si != sie)
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
    WeakSet<Chunk*>::iterator cie = active_chunks.end();
    while (ci != cie)
    {
        Chunk* c = *ci;

        if (c->particles.empty())
        {
            active_chunks.erase(ci, cie);
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

            signed int dx = abs(new_x - old_x);
            signed int dy = abs(new_y - old_y);

            signed int sx = old_x < new_x ? 1 : -1;
            signed int sy = old_y < new_y ? 1 : -1;

            signed int err = dx - dy;

            signed int x = old_x;
            signed int y = old_y;

            while (true)
            {
                // TODO: optimization
                signed int e2 = err * 2;
                if (e2 > -dy)
                {
                    err -= dy;
                    x += sx;
                }

                if (!(x == new_x && y == new_y) && e2 < dx)
                {
                    err += dx;
                    y += sy;
                }

                Chunk *nc = c;
                unsigned int i = new_x + new_y * CHUNK_SIZE;
                if (i >= CHUNK_SIZE * CHUNK_SIZE)
                {
                    if (new_x < 0)
                        {nc = nc->neighbor(this, Chunk::neighbor_left); i += CHUNK_SIZE;}
                    else if (new_x > CHUNK_SIZE)
                        {nc = nc->neighbor(this, Chunk::neighbor_right); i -= CHUNK_SIZE;}
                    if (new_y < 0)
                        {nc = nc->neighbor(this, Chunk::neighbor_up); i += CHUNK_SIZE * CHUNK_SIZE;}
                    else if (new_y > CHUNK_SIZE)
                        {nc = nc->neighbor(this, Chunk::neighbor_down); i -= CHUNK_SIZE * CHUNK_SIZE;}
                }

                if (nc != c)
                {
                    if (nc->particles.empty())
                    {
                        active_chunks.insert(nc, ci, cie);
                    }

                    c->particles.erase(pi);
                    nc->particles.insert(p);

                    if (c->particles.empty())
                    {
                        active_chunks.erase(ci);
                        ci--;
                    }
                }

                Cell *ncl = nc->cells + i;
                switch (ncl->state)
                {
                case Cell::t_air:
                    view.update_pixel(c->x + old_x, c->y + old_y, 0, 0, 0);
                    view.update_pixel(c->x + new_x, c->y + new_y, p->type->r, p->type->g, p->type->b);

                    cl->state = Cell::t_air;
                    ncl->state = Cell::t_dynamic;
                    ncl->particle = p;
                    break;

                case Cell::t_static:
                    std::cout << "static collision" << std::endl;

                    // TODO: Should calculate new velocity from the surface normal
                    p->vx = -p->vx;
                    p->vy = -p->vy;

                    p->x += p->vx;
                    p->y += p->vy;

                    new_x = p->x;
                    new_y = p->y;
                    break;

                case Cell::t_dynamic:
                    Particle *p2 = ncl->particle;

                    // Bounce

                    /*
                    // Unoptimized, without mass
                    float dx = p2->x - p->x;
                    float dy = p2->y - p->y;
                    float d = sqrt(dx * dx + dy * dy);
                    dx /= d;
                    dy /= d;

                    float dvx = p2->vx - p->vx;
                    float dvy = p2->vy - p->vy;

                    float i = dvx * dx + dvy * dy;
                    float ix = dx * i;
                    float iy = dy * i;
                    */

                    float dx = p2->x - p->x;
                    float dy = p2->y - p->y;
                    float d2 = dx * dx + dy * dy;

                    float dvx = p2->vx - p->vx;
                    float dvy = p2->vy - p->vy;

                    float i = (dvx * dx + dvy * dy) / (d2 * (p->type->mass + p2->type->mass));
                    float ix = dx * i;
                    float iy = dy * i;

                    p->vx -= ix * p2->type->mass;
                    p->vy -= iy * p2->type->mass;

                    if (p->solid)
                    {
                        p->solid->apply_impulse(p->x, p->y, ix * p->type->mass, py * p->type->mass);
                    }
                    else
                    {
                        p2->vx += ix * p->type->mass;
                        p2->vy += ix * p->type->mass;
                    }
                    break;
                }

                if (x == new_x && y == new_y)
                {
                    set_particle(x, y);
                    break;
                }
            }

            pi++;
        }

        ci++;
    }

    cur_timestep++;
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
