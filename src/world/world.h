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
#include "util/offsetvector.h"
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

    struct Node
    {
        enum Contents {all_air, subdiv, defined};
        Node *parent;

        union
        {
            // subdiv
            Node *children;

            // defined
            Particle *cells;
        };
    };

    OffsetVector<OffsetVector<Node>> grid;

    /*
    Solids should only search for solids larger than itself:
        Imagine an explosion that creates hundreds of large solids (n solids, with radius r) and thousands of small particles (m particles, with radius 1).
        If the large solids collided with the smaller particles, each large solid would have to test many small particles inside it's aabb. About (4-pi)*r^2*n tests.
        In addition, the small solids would have to test about 1 or 2 large particles.

    Reference frame shifting

    Foreach solid:
        If velocity < threshold: // Or detect when neighbors are being not kept long enough
            Neighbor trimming - doesn't need to happen every tick:
                Foreach neighbor:
                    If new_aabb doesn't intersect neighbor_aabb:
                        Remove neighbor from list
            If last_aabb doesn't completely contain new_aabb:
                Calculate test_aabb = (old_aabb : new_aabb) - last_aabb
                Find all larger solids that intersect test_aabb
                Add each intersecting solid to the neighbor list
                Set last_aabb = new_aabb
                Expand last_aabb by ~0.25 px // This is so that when the solid is at rest, diff_aabb will be zero
        Else velocity > threshold:
            Calculate test_aabb = (old_aabb : new_aabb)
            Find all larger solids that intersect test_aabb
            Clear neighbor list
            Add each intersecting solid to the neighbor list

        Foreach neighbor:
            Run detailed collision detection
                Calculate common_aabb = new_aabb & neighbor_aabb
                Intialize sector binary search based on the common_aabb
    */

    // if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 && RectA.Y1 < RectB.Y2 && RectA.Y2 > RectB.Y1)


    /*
    Particle queries:

        Get all particles within rad
        Get all particles of type within rad
        Get all particles touching line segment

    */
};

#endif // WORLD_H
