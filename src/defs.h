#ifndef DEFS_H
#define DEFS_H

//#define NDEBUG
#include <assert.h>

// Simulation chunk size in pixels. Must be a power of 2.
#define CHUNK_SIZE 64

// Render chunk size in pixels. Must be a power of 2 greater than or equal to CHUNK_SIZE.
#define VIEW_CHUNK_SIZE 128

// The number of chunks to allocate when more are needed.
#define CHUNK_ALLOC_SIZE 256

// Simulation gravity and friction.
#define GRAVITY 0.1f
#define DAMPING 0.99f
#define ANGULAR_DAMPING 0.99f

#define ROTATION_BINS 360

// The target seconds per frame.
#define TARGET_SPF 0.02


#endif // DEFS_H
