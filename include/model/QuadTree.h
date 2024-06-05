#ifndef MODEL_QUADTREE_H
#define MODEL_QUADTREE_H

#include <MLV/MLV_all.h>
#include "../model/Pixel.h"

typedef struct _quadnode_ {
    pixel color;
    struct _quadnode_ *northwest;
    struct _quadnode_ *northeast;
    struct _quadnode_ *southwest;
    struct _quadnode_ *southeast;
    double error;
} quadnode;

double calculate_error(MLV_Image *image, int startX, int startY, int width, int height, pixel average);
quadnode *create_quadnode(MLV_Image *image, int startX, int startY, int width, int height);

#endif
