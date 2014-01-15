#ifndef AABB_H
#define AABB_H

struct Aabb
{
    union
    {
        float arr[4];
        struct
        {
            float up;
            float down;
            float left;
            float right;
        };
    };
};

#endif // AABB_H
