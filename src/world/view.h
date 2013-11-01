#ifndef VIEW_H
#define VIEW_H

#include "gl.h"

class View
{
public:
    View();

    void set_size(unsigned int width, unsigned int height);
    void set_transform(unsigned int x, unsigned int y);

protected:
    unsigned int width;
    unsigned int height;

    unsigned int x;
    unsigned int y;
    unsigned int mx;
    unsigned int my;

    GLuint tex;
};

#endif // VIEW_H
