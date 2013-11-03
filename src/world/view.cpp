#include "view.h"

View::View()
{
    GLubyte *data = new GLubyte[500 * 500 * 3];
    int i = 0;
    while (i < 500 * 500 * 3)
    {
        //data[i] = rand() % 256;
        i++;
    }
}

void View::update_pixel(signed int x, signed int y, unsigned char r, unsigned char g, unsigned char b)
{
    //std::cout << x << " " << y << " " << (unsigned int)r << " " << (unsigned int)g << " " << (unsigned int)b << std::endl;

    div_t dx = div(x, VIEW_CHUNK_SIZE);
    div_t dy = div(y, VIEW_CHUNK_SIZE);

    if (dx.quot != cur_vc_id.vcx || dy.quot != cur_vc_id.vcy)
    {
        cur_vc_id.vcx = dx.quot;
        cur_vc_id.vcy = dy.quot;

        cur_vc = &vcs[cur_vc_id];
        if (!cur_vc->tex)
        {
            glGenTextures(1, &cur_vc->tex);
            glBindTexture(GL_TEXTURE_2D, cur_vc->tex);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEW_CHUNK_SIZE, VIEW_CHUNK_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, cur_vc->data);
        }

        if (!cur_vc->update_max_x)
        {
            update_vcs.push_back(cur_vc);
        }
    }

    GLubyte *d = cur_vc->data + (dx.rem + dy.rem * VIEW_CHUNK_SIZE) * 3;
    *d++ = r;
    *d++ = g;
    *d++ = b;

    if (dx.rem < cur_vc->update_min_x) {cur_vc->update_min_x = dx.rem;}
    if (dy.rem < cur_vc->update_min_y) {cur_vc->update_min_y = dy.rem;}
    if (dx.rem >= cur_vc->update_max_x) {cur_vc->update_max_x = dx.rem + 1;}
    if (dy.rem >= cur_vc->update_max_y) {cur_vc->update_max_y = dy.rem + 1;}
}

void View::draw()
{
    // Update changed textures
    std::vector<ViewChunk*>::iterator i1 = update_vcs.begin();
    while (i1 != update_vcs.end())
    {
        ViewChunk* vc = *i1;

        glBindTexture(GL_TEXTURE_2D, vc->tex);
        //glTexSubImage2D(GL_TEXTURE_2D, 0, vc->update_min_x, vc->update_min_y, vc->update_max_x - vc->update_min_x, vc->update_max_y - vc->update_min_y, GL_RGB, GL_UNSIGNED_BYTE, vc->data);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VIEW_CHUNK_SIZE, VIEW_CHUNK_SIZE, GL_RGB, GL_UNSIGNED_BYTE, vc->data);

        vc->update_min_x = VIEW_CHUNK_SIZE;
        vc->update_min_y = VIEW_CHUNK_SIZE;
        vc->update_max_x = 0;
        vc->update_max_y = 0;

        i1++;
    }

    update_vcs.clear();
    cur_vc_id = ViewChunkId();

    // Draw all textures
    glEnable(GL_TEXTURE_2D);

    std::map<ViewChunkId, ViewChunk>::iterator i2 = vcs.begin();
    while (i2 != vcs.end())
    {
        signed int cx = i2->first.vcx * VIEW_CHUNK_SIZE;
        signed int cy = i2->first.vcy * VIEW_CHUNK_SIZE;
        signed int mcx = cx + VIEW_CHUNK_SIZE;
        signed int mcy = cy + VIEW_CHUNK_SIZE;

        glBindTexture(GL_TEXTURE_2D, i2->second.tex);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2i(cx , cy );
        glTexCoord2f(0, 1); glVertex2i(cx , mcy);
        glTexCoord2f(1, 1); glVertex2i(mcx, mcy);
        glTexCoord2f(1, 0); glVertex2i(mcx, cy );
        glEnd();

        i2++;
    }

    glDisable(GL_TEXTURE_2D);
}
