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

Particle *Terrain::make_chunk(signed int cx, signed int cy)
{
    Particle *res = new Particle[CHUNK_SIZE * CHUNK_SIZE];
    Particle *res_i = res;

    float ax = cx * CHUNK_SIZE;
    float ay = cy * CHUNK_SIZE;
    float bx = ax + CHUNK_SIZE;
    float by = ay + CHUNK_SIZE;

    float y = ay;
    while (y < by)
    {
        float x = ax;
        while (x < bx)
        {
            float val = noise.main->octave_noise_2d(1, 0.5, 0.1, x, y) - y / 4.0;
            if (val > 0.0)
            {
                res_i->type = 0;
                res_i->solid = 0;
            }
            else
            {
                res_i->type = 1;
                res_i->solid = 0;
            }
            res_i++;

            x++;
        }
        y++;
    }

    return res;
}
