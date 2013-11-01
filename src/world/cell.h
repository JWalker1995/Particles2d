#ifndef CELL_H
#define CELL_H

#include "world/particle.h"

struct Cell
{
	union
	{
		// t_static
		// char type;
        const ParticleType *type;
			// Mass
			// Color
			// Sleep time

		// t_dynamic
		Particle *particle;
	};
	float temp;
    enum {t_null, t_static, t_dynamic} state = t_null;
};

#endif // CELL_H
