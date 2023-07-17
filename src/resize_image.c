#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
     int near_x = round(x);
    int near_y= round(y);
    return get_pixel(im,near_x , near_y, c);
    return 0;
}

image nn_resize(image im, int w, int h)
{
    image resized = make_image(wt, ht, im.c);
    float x_ratio = (float)(im.wd - 1) / (wd - 1);
    float y_ratio = (float)(im.ht - 1) / (ht- 1);

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < ht; y++)
        {
            for (int x = 0; x < wd; x++)
            {
                float px = x * x_ratio;
                float py = y * y_ratio;
                float val = nn_interpolate(im, px, py, c);
                set_pixel(resized, x, y, c, val);
            }
        }
    }
    return resized;
    // return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
   int ix = floor(x);
    int iy = floor(y);

    float dx = x - near_x;
    float dy = y - near_y;

    float v1 = get_pixel(im, near_x, near_y, c);
    float v2 = get_pixel(im, near_x + 1, near_y, c);
    float v3 = get_pixel(im, near_x, near_y + 1, c);
    float v4 = get_pixel(im, near_x + 1, near_y + 1, c);

    float q1 = v1 * (1 - dx) + v2 * dx;
    float q2 = v3 * (1 - dx) + v4 * dx;

    return q1 * (1 - dy) + q2 * dy;
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
   int ix = floor(x);
    int iy = floor(y);

    float dx = x - near_x;
    float dy = y - near_y;

    float v1 = get_pixel(im, near_x, near_y, c);
    float v2 = get_pixel(im, near_x + 1, near_y, c);
    float v3 = get_pixel(im, near_x, near_y + 1, c);
    float v4 = get_pixel(im, near_x + 1, near_y + 1, c);

    float q1 = v1 * (1 - dx) + v2 * dx;
    float q2 = v3 * (1 - dx) + v4 * dx;

    return q1 * (1 - dy) + q2 * dy;
    return make_image(1,1,1);
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);
    float x_ratio = (float)(im.w - 1) / (w - 1);
    float y_ratio = (float)(im.h - 1) / (h - 1);

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                float px = x * x_ratio;
                float py = y * y_ratio;
                float val = bilinear_interpolate(im, px, py, c);
                set_pixel(resized, x, y, c, val);
            }
        }
    }

    return resized;
    return make_image(1,1,1);
}

