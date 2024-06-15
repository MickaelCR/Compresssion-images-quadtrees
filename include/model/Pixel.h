#ifndef MODEL_PIXEL_H
#define MODEL_PIXEL_H

#include <MLV/MLV_all.h>

typedef struct _pixel_ {
    int red;
    int green;
    int blue;
    int alpha;
} pixel;

double distance(pixel pixel1, pixel pixel2);
int equal_pixels(pixel pixel1, pixel pixel2);
pixel read_image(MLV_Image *image, int startX, int startY, int size);
MLV_Color to_MLV_Color(pixel pixel);

#endif
