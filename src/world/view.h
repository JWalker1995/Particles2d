#ifndef VIEW_H
#define VIEW_H

#include "defs.h"
#include "gl.h"

#include <limits>

#include <vector>
#include <map>

#include <iostream>

class View
{
public:
    View();

    void set_size(unsigned int width, unsigned int height);
    void set_transform(unsigned int x, unsigned int y);

    void update_pixel(signed int x, signed int y, unsigned char r, unsigned char g, unsigned char b);

    void draw();

protected:
    unsigned int width;
    unsigned int height;

    unsigned int x;
    unsigned int y;


    struct ViewChunkId
    {
        ViewChunkId()
            : vcx(std::numeric_limits<signed int>::max())
            , vcy(std::numeric_limits<signed int>::max())
        {}

        ViewChunkId(signed int vcx, signed int vcy)
            : vcx(vcx)
            , vcy(vcy)
        {}

        signed int vcx;
        signed int vcy;

        bool operator<(const ViewChunkId &vci) const
        {
            return vcx == vci.vcx ? vcy < vci.vcy : vcx < vci.vcx;
        }
        bool operator==(const ViewChunkId &vci)
        {
            return vcx == vci.vcx && vcy == vci.vcy;
        }
    };
    struct ViewChunk
    {
        GLuint tex = 0;
        GLubyte data[VIEW_CHUNK_SIZE * VIEW_CHUNK_SIZE * 3];

        unsigned int update_min_x = VIEW_CHUNK_SIZE;
        unsigned int update_min_y = VIEW_CHUNK_SIZE;
        unsigned int update_max_x = 0;
        unsigned int update_max_y = 0;
    };

    std::map<ViewChunkId, ViewChunk> vcs;
    ViewChunkId cur_vc_id;
    ViewChunk *cur_vc;
    std::vector<ViewChunk*> update_vcs;
};

#endif // VIEW_H
