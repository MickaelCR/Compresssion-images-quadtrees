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
    int x;
    int y;
    int size;
    double error;
} quadnode;

double calculate_error(MLV_Image *image, int startX, int startY, int size, pixel average);
quadnode *create_quadnode(MLV_Image *image, int startX, int startY, int size);
void subdivide(quadnode *node, MLV_Image *image);
quadnode *find_max_error(quadnode *node);

#endif
