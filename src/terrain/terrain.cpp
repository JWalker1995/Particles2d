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
    float ex = chunk->x + CHUNK_SIZE;
    float ey = chunk->y + CHUNK_SIZE;

    Cell *cell = chunk->cells;

    float y = chunk->y;
    while (y < ey)
    {
        float x = chunk->x;
        while (x < ex)
        {
            float val = noise.main->octave_noise_2d(1, 0.5, 0.1, x, y) + (y - 128.0f) / 16.0f;
            if (val < 0.0)
            {
                cell->state = Cell::t_air;
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
