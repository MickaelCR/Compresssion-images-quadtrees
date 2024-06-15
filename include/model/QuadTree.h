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

#include "../model/Heap.h"

quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap);
quadnode *create_empty_quadnode(int x, int y, int size);
void subdivide(quadnode *node, MLV_Image *image, max_heap *heap);
quadnode *find_max_error(quadnode *node);
void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize);
void save_quadtree(quadnode *tree, const char *filename);
void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize);
quadnode *load_quadtree(const char *filename);
#endif
