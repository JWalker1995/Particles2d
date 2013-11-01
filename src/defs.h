#ifndef DEFS_H
#define DEFS_H

//#define NDEBUG
#include <assert.h>

// Chunk size in pixels
#define CHUNK_SIZE 64

// The number of chunks to allocate when more are needed
#define CHUNK_ALLOC_SIZE 256

// Simulation gravity and friction
#define GRAVITY 0.1f
#define DAMPING 0.99f
#define ANGULAR_DAMPING 0.99f

// The target seconds per frame
#define TARGET_SPF 0.02

typedef float Scalar;

#endif // DEFS_H
