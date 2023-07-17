#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
   float sum = 0;

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                sum += get_pixel(im, x, y, c);
            }
        }
    }
    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                set_pixel(im, x, y, c, get_pixel(im, x, y, c) / sum);
            }
        }
    }
}

image make_box_filter(int w)
{
   image filter = make_image(w, w, 1);

    for (int y = 0; y < w; y++)
    {
        for (int x = 0; x < w; x++)
        {
            set_pixel(filter, x, y, 0, 1.0 / (w * w));
        }
    }

    return filter;
    return make_image(1,1,1);
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == 1 || filter.c == im.c);

    int pad = filter.w / 2;
    int out_w = im.w;
    int out_h = im.h;
    int out_c = (preserve && (filter.c == im.c)) ? im.c : 1;

    image output = make_image(out_w, out_h, out_c);
    for (int c = 0; c < out_c; c++)
    {
        for (int y = 0; y < out_h; y++)
        {
            for (int x = 0; x < out_w; x++)
            {
                float sum = 0;

                for (int fy = 0; fy < filter.h; fy++)
                {
                    for (int fx = 0; fx < filter.w; fx++)
                    {
                        int ix = x + fx - pad;
                        int iy = y + fy - pad;

                        float filter_val = get_pixel(filter, fx, fy, 0);
                        float image_val = (preserve && (filter.c == im.c)) ? get_pixel(im, ix, iy, c) : get_pixel(im, ix, iy, 0);

                        sum += filter_val * image_val;
                    }
                }
                 set_pixel(output, x, y, c, sum);
            }
        }
    }

    return output;
     return make_image(1,1,1);
}

image make_highpass_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return make_image(1,1,1);
}

image make_emboss_filter()
{
  image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int size = ceil(6 * sigma);

    if (size % 2 == 0)
    {
        size++;
    }

    image filter = make_image(size, size, 1);

    float sum = 0;
    int pad = size / 2;
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            int ix = x - pad;
            int iy = y - pad;
            float value = expf(-(ix * ix + iy * iy) / (2 * sigma * sigma)) / (TWOPI * sigma * sigma);
            set_pixel(filter, x, y, 0, value);
            sum += value;
        }
    }

    for (int i = 0; i < size * size; i++)
    {
        filter.data[i] /= sum;
    }

    return filter;
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
  assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int i = 0; i < a.w * a.h * a.c; i++)
    {
        result.data[i] = a.data[i] + b.data[i];
    }

    return result;
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
   assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int i = 0; i < a.w * a.h * a.c; i++)
    {
        result.data[i] = a.data[i] - b.data[i];
    }

    return result;
    return make_image(1,1,1);
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
    return make_image(1,1,1);
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
  float min_val = im.data[0];
    float max_val = im.data[0];

    for (int i = 1; i < im.w * im.h * im.c; i++)
    {
        if (im.data[i] < min_val)
            min_val = im.data[i];
        if (im.data[i] > max_val)
            max_val = im.data[i];
    }

    float range = max_val - min_val;

    if (range == 0)
    {
        memset(im.data, 0, im.w * im.h * im.c * sizeof(float));
    }
    else
    {
        for (int i = 0; i < im.w * im.h * im.c; i++)
        {
            im.data[i] = (im.data[i] - min_val) / range;
        }
    }
}

}

image *sobel_image(image im)
{
    image *sobel = calloc(2, sizeof(image));

    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    sobel[0] = convolve_image(im, gx_filter, 0);
    sobel[1] = convolve_image(im, gy_filter, 0);

    return sobel;
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
   image *sobel = sobel_image(im);
    image mag = make_image(im.w, im.h, 1);
    image colorized = make_image(im.w, im.h, 3);

    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float magnitude = sqrtf(powf(get_pixel(sobel[0], x, y, 0), 2) + powf(get_pixel(sobel[1], x, y, 0), 2));
            set_pixel(mag, x, y, 0, magnitude);
        }
    }

    feature_normalize(mag);
   for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float magnitude = get_pixel(mag, x, y, 0);
            float hue = (atan2f(get_pixel(sobel[1], x, y, 0), get_pixel(sobel[0], x, y, 0)) + M_PI) / TWOPI;
            set_pixel(colorized, x, y, 0, hue);
            set_pixel(colorized, x, y, 1, magnitude);
            set_pixel(colorized, x, y, 2, magnitude);
        }
    }

    hsv_to_rgb(colorized);

    free_image(sobel[0]);
    free_image(sobel[1]);
    free(sobel);

    return colorized;
    return make_image(1,1,1);
}
