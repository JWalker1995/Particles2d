#include "chunk.h"

std::vector<Chunk*> Chunk::chunks;

unsigned int Chunk::next_chunk = CHUNK_ALLOC_SIZE;
unsigned int Chunk::allocated = 0;

Chunk::Chunk(World *world, signed int cx, signed int cy)
    : x(cx)
    , y(cy)
{
    world->initialize_chunk(this);
}

Chunk *Chunk::neighbor(World *world, Neighbor i)
{
    if (neighbors[i]) {return neighbors[i];}

    // Calculate target cx and cy.
    signed int tcx = x;
    signed int tcy = y;
    switch (i)
    {
    case neighbor_up:
        tcy -= CHUNK_SIZE; break;
    case neighbor_down:
        tcy += CHUNK_SIZE; break;
    case neighbor_left:
        tcx -= CHUNK_SIZE; break;
    case neighbor_right:
        tcx += CHUNK_SIZE; break;
    }

    // Loop through each chunk.
    // If we find one that matches the target cx and cy, set neighbors and return.
    // TODO: reverse iterating (might be faster because newer chunks are more likely to be closer).
    std::vector<Chunk*>::iterator ci = chunks.begin();
    while (ci != chunks.end())
    {
        Chunk *chunk = *ci;
        ci++;
        Chunk *chunk_e = chunk + (ci != chunks.end() ? CHUNK_ALLOC_SIZE : next_chunk);
        while (chunk < chunk_e)
        {
            if (chunk->x == tcx && chunk->y == tcy)
            {
                neighbors[i] = chunk;
                chunk->neighbors[i ^ 1] = this;
                return chunk;
            }
            chunk++;
        }
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

void Chunk::set_pixel(signed int px, signed int py)
{
}
