#include "terrain.h"

Terrain::Terrain(std::uint32_t seed, int version)
{
    Random master(seed);
    // Advance master a few times to get away from the seed
    master.discard(64 + seed % 64);

    this->seed = seed;
    this->version = version;
    this->rand = master();

    this->noise.main = new SimplexNoise(master);
}

Terrain::~Terrain()
{
    delete noise.main;
}

void Terrain::make_chunk(Chunk *chunk)
{
    float ax = chunk->cx * CHUNK_SIZE;
    float ay = chunk->cy * CHUNK_SIZE;
    float bx = ax + CHUNK_SIZE;
    float by = ay + CHUNK_SIZE;

    Cell *cell = chunk->cells;

    float y = ay;
    while (y < by)
    {
        float x = ax;
        while (x < bx)
        {
            float val = noise.main->octave_noise_2d(1, 0.5, 0.1, x, y) - y / 4.0;
            if (val < 0.0)
            {
                cell->state = Cell::t_static;
                cell->temp = 0.0f;
                cell->type = ParticleType::air;
            }
            else
            {
                cell->state = Cell::t_static;
                cell->temp = 0.0f;
                cell->type = ParticleType::dirt;
            }
            cell++;

            x++;
        }
        y++;
    }
}
