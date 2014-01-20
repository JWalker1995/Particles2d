#ifndef BROADPHASE_H
#define BROADPHASE_H

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

#include "object.h"
#include "util/offsetvector.h"

class Broadphase
{
    void insert(Object *obj)
    {
        signed int ty1 = obj->y - obj->rel_aabb[Direction::up   ];
        signed int ty2 = obj->y + obj->rel_aabb[Direction::down ];
        signed int tx1 = obj->x - obj->rel_aabb[Direction::left ];
        signed int tx2 = obj->x + obj->rel_aabb[Direction::right];
    }

    template<typename Iter>
    void tick(Iter begin, Iter end)
    {
        std::vector<Object*> ignore;

        while (begin != end)
        {
            Object &obj = *begin;

            unsigned int scale = obj.scale;

            signed int left  = floor(obj.aabb[Direction::left ]);
            signed int up    = floor(obj.aabb[Direction::up   ]);
            signed int right = floor(obj.aabb[Direction::right]);
            signed int down  = floor(obj.aabb[Direction::down ]);

            unsigned int tx1 = (unsigned int)(left  + 0xF0000000) >> scale;
            unsigned int ty1 = (unsigned int)(up    + 0xF0000000) >> scale;
            unsigned int tx2 = (unsigned int)(right + 0xF0000000) >> scale;
            unsigned int ty2 = (unsigned int)(down  + 0xF0000000) >> scale;

            while (scale < grids.size())
            {
                Grid &grid = grids[scale];

                ignore.clear();
                ignore.push_back(&obj);

                collide_objects(obj, grid[tx1][ty1], ignore);
                if (tx1 != tx2)
                {
                    collide_objects(obj, grid[tx2][ty1], ignore);
                    if (ty1 != ty2)
                    {
                        collide_objects(obj, grid[tx1][ty2], ignore);
                        collide_objects(obj, grid[tx2][ty2], ignore);
                    }
                }
                else if (ty1 != ty2)
                {
                    collide_objects(obj, grid[tx1][ty2], ignore);
                }

                tx1 >> 1;
                ty1 >> 1;
                tx2 >> 1;
                ty2 >> 1;

                scale++;
            }

            begin++;
        }
    }

protected:
    typedef OffsetVector<OffsetVector<std::vector<Object*>>> Grid;

    std::vector<Grid> grids;

    std::vector<Object*> objects;

    void collide_objects(const Object &obj, const std::vector<Object*> &objects, std::vector<Object*> &ignore)
    {
        std::vector<Object*>::iterator i = objects.begin();
        while (i != objects.end())
        {
            Object *obj2 = *i;

            if (       obj.aabb[Direction::up   ] < obj2->aabb[Direction::down ]
                    && obj.aabb[Direction::down ] > obj2->aabb[Direction::up   ]
                    && obj.aabb[Direction::left ] < obj2->aabb[Direction::right]
                    && obj.aabb[Direction::right] > obj2->aabb[Direction::left ])
            {
                if (std::find(ignore.begin(), ignore.end(), obj2) == ignore.end())
                {
                    ignore.push_back(obj2);
                    Object::collide(obj, obj2);
                }
            }
        }
    }

    /*
    Chunk *find_chunk(Chunk *root, signed int tx, signed int ty)
    {
        static signed int last_tx = std::numeric_limits<signed int>::max();
        static signed int last_ty = std::numeric_limits<signed int>::max();
        static Chunk *last_return;

        if (tx == last_tx && ty == last_ty)
        {
            return last_return;
        }
        else
        {
            last_tx = tx;
            last_ty = ty;
        }

        ChunkSearch search(tx, ty);
        std::priority_queue<Chunk*, std::vector<Chunk*>, ChunkSearch> heads(search);
        heads.push_back(roots[obj->scale]);

        while (!heads.empty())
        {
            Chunk *top = heads.top();
            if (tx == top->x && ty == top->y)
            {
                last_return = top;
                return top;
            }
            heads.pop();

            unsigned int i = 0;
            while (i < 4)
            {
                if (top->neighbors[i]) {heads.push(top->neighbors[i]);}
                i++;
            }
        }

        // Create node
    }

    std::vector<Chunk*> roots;
    // roots[0] - 1 px chunks
    // roots[1] - 2 px chunks
    // roots[2] - 4 px chunks
    // roots[3] - 8 px chunks
    // ...
    */

private:
    /*
    struct ChunkSearch
    {
        ChunkSearch(const signed int tx, const signed int ty)
            : tx(tx)
            , ty(ty)
        {}

        signed int tx;
        signed int ty;

        bool operator() (const Chunk *c1, const Chunk *c2) const
        {
            signed int dx1 = tx - c1->x;
            signed int dy1 = ty - c1->y;
            signed int dx2 = tx - c2->x;
            signed int dy2 = ty - c2->y;

            return dx1 * dx1 + dy1 * dy1 < dx2 * dx2 + dy2 * dy2;
        }
    };
    */
};

#endif // BROADPHASE_H
