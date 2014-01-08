#ifndef CELL_H
#define CELL_H

#include "world/particle.h"

struct Cell
{
	union
	{
		// state_static:
		// char type;
        const ParticleType *type;
			// Mass
			// Color
			// Sleep time

		// state_solid:
		Solid *solid;
		Particle *particle;		

		// state_particle:
		Particle particle;
	};
	float temp;
    enum {state_air, state_static, state_solid, state_particle} state = state_air;
};

#endif // CELL_H
