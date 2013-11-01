#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <set>
#include <deque>

#include "defs.h"
#include "gl.h"

#include "world/cell.h"
#include "terrain/terrain.h"
#include "util/offsetvector.h"

class World
{
public:
    World(int seed, int version);

    void tick(float time);

    GLuint get_texture() {return tex;}

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

protected:
    Cell &get_cell(signed int x, signed int y);

    //Chunk &get_bin(signed int cx, signed int cy);
    //Particle *get_particle(signed int x, signed int y);

    ProximityTestIterator proximity_test(float x, float y, float rad);

    struct Chunk
    {
        Chunk(signed int cx, signed int cy)
            : cx(cx)
            , cy(cy)
        {}

        signed int cx;
        signed int cy;

        /*
        float x;
        float y;
        */

        Cell cells[CHUNK_SIZE * CHUNK_SIZE];

        std::vector<Particle*> particles;

        Chunk *neighbor(unsigned int i)
        {
            if (neighbors[i]) {return neighbors[i];}

            // Calculate target cx and cy.
            signed int tcx = cx;
            signed int tcy = cy;
            switch (i)
            {
            case 0:
                // Up
                tcy--; break;
            case 1:
                // Down
                tcy++; break;
            case 2:
                // Left
                tcx--; break;
            case 3:
                // Right
                tcy++; break;
            }

            // Loop through each chunk.
            // If we find one that matches the target cx and cy, set neighbors and return.
            // TODO: reverse iterating (might be faster because newer chunks are more likely to be closer).
            std::vector<Chunk*>::iterator i = chunks.begin();
            while (i != chunks.end())
            {
                Chunk *i2 = *i;
                Chunk *i2e = i2 + CHUNK_ALLOC_SIZE;
                while (i2 < i2e)
                {
                    if (i2->cx == tcx && i2->cy == tcy)
                    {
                        neighbors[i] = i2;
                        i2->neighbors[i ^ 1] = this;
                        return i2;
                    }
                    i2++;
                }
                i++;
            }

            // If there are no chunks at the target cx and cy, make one.
            Chunk *chunk = new Chunk(tcx, tcy);
            neighbors[i] = chunk;
            chunk->neighbors[i ^ 1] = this;
            return chunk;
        }

    protected:
        // up, down, left, right
        Chunk *neighbors[4];

    public:
        void *operator new(size_t s) throw (std::bad_alloc)
        {
            assert(s == sizeof(Chunk));

            if (next_chunk == CHUNK_ALLOC_SIZE)
            {
                chunks.push_back(new Chunk[CHUNK_ALLOC_SIZE]);
                next_chunk = 0;
            }

            allocated++;
            return chunks.back() + next_chunk;
        }

        // Most chunks won't be de-allocated until the end of the program.
        void operator delete(void *chunk, size_t s)
        {
            allocated--;
            if (!allocated)
            {
                std::vector<Chunk*>::iterator i = chunks.begin();
                while (i != chunks.end())
                {
                    delete[] *i;
                    i++;
                }
                next_chunk = CHUNK_ALLOC_SIZE;
            }
        }

    protected:
        static std::vector<Chunk*> chunks;
        static unsigned int next_chunk = CHUNK_ALLOC_SIZE;
        static unsigned int allocated = 0;
    };

    Chunk *root;

    //OffsetVector<OffsetVector<Cell>> cells;

    std::deque<Solid*> solids;
    //std::deque<Particle*> particles;

    Terrain terrain;

    GLuint tex;

    /*
    Particle queries:

        Get all particles within rad
        Get all particles of type within rad
        Get all particles touching line segment

    */
};

#endif // WORLD_H
