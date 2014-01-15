#ifndef OBJECT_H
#define OBJECT_H

#include "util/direction.h"

class Object
{
    float x;
    float y;
    float rad;
    float mass;

    float rel_aabb[4];

    unsigned int scale;
};

#endif // OBJECT_H
