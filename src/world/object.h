#ifndef OBJECT_H
#define OBJECT_H

#include "util/direction.h"

class Object
{
    float x;
    float y;
    float mass;

    float rad;
    float rel_aabb[4];
    float aabb[4];

    unsigned int scale;

    static void collide(Object &obj1, Object &obj2)
    {
    }
};

#endif // OBJECT_H
