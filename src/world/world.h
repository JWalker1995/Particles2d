#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <set>
#include <deque>

#include "defs.h"
#include "gl.h"

#include "world/view.h"
#include "terrain/terrain.h"
#include "world/chunk.h"
#include "world/solid.h"
//#include "util/offsetvector.h"
#include "util/weakset.h"

class Terrain;

class World
{
public:
    World(int seed, int version);

    void tick(float time);
    void draw();

    /*
    struct ProximityTestIterator
    {
        Particle *next();

    protected:
        ProximityTestIterator(World *world, float x, float y, float rad);

        World *world;

        float x;
        float y;
        float rad;

        signed int bin_x;
        signed int bin_ex;

        signed int bin_sy;
        signed int bin_y;
        signed int bin_ey;

        Chunk *bin;

        Particle *bin_particles;
        unsigned int particle_i;

        std::set<Solid*>::iterator bin_solids_i;
        std::set<Solid*>::iterator bin_solids_e;

        std::vector<Solid::SolidParticle>::iterator solid_particle_i;
        std::vector<Solid::SolidParticle>::iterator solid_particle_e;
    };
    */

    void initialize_chunk(Chunk *chunk);

protected:
    //Cell &get_cell(signed int x, signed int y);

    //Chunk &get_bin(signed int cx, signed int cy);
    //Particle *get_particle(signed int x, signed int y);

    //ProximityTestIterator proximity_test(float x, float y, float rad);

    WeakSet<Solid*> solids;

    Chunk *root;
    WeakSet<Chunk*> active_chunks;

    //OffsetVector<OffsetVector<Cell>> cells;

    //std::deque<Particle*> particles;


    // TODO: shouldn't have to be a pointer
    Terrain *terrain;

    View view;

    /*
    Particle queries:

        Get all particles within rad
        Get all particles of type within rad
        Get all particles touching line segment

    */
};

#endif // WORLD_H
