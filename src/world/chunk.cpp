#include "chunk.h"

std::vector<Chunk*> Chunk::chunks;

unsigned int Chunk::next_chunk = CHUNK_ALLOC_SIZE;
unsigned int Chunk::allocated = 0;

Chunk::Chunk(World *world, signed int cx, signed int cy)
    : cx(cx)
    , cy(cy)
{
    world->initialize_chunk(this);
}

Chunk *Chunk::neighbor(World *world, unsigned int i)
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
    std::vector<Chunk*>::iterator ci = chunks.begin();
    while (ci != chunks.end())
    {
        Chunk *chunk = *ci;
        Chunk *chunk_e = chunk + CHUNK_ALLOC_SIZE;
        while (chunk < chunk_e)
        {
            if (chunk->cx == tcx && chunk->cy == tcy)
            {
                neighbors[i] = chunk;
                chunk->neighbors[i ^ 1] = this;
                return chunk;
            }
            chunk++;
        }
        ci++;
    }

    // If there are no chunks at the target cx and cy, make one.
    Chunk *chunk = new Chunk(world, tcx, tcy);
    neighbors[i] = chunk;
    chunk->neighbors[i ^ 0] = 0;
    chunk->neighbors[i ^ 1] = this;
    chunk->neighbors[i ^ 2] = 0;
    chunk->neighbors[i ^ 3] = 0;
    return chunk;
}
