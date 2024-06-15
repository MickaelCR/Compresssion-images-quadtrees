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
    int id;
} quadnode;

#include "../model/Heap.h"

quadnode *create_quadnode_no_image(int id);


void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, max_heap *heap, int isBW);

void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, int isBW);
quadnode *find_max_error(quadnode *node);
quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap);
void subdivide(quadnode *node, MLV_Image *image, max_heap *heap);
void free_quadnode(quadnode *node);
void minimise_quadtree(quadnode *tree);

void assign_ids(quadnode *node, int *current_id);
void write_minimized_node_to_file(quadnode *node, FILE *fptr);

void save_quadtree_unminimized(quadnode *tree, const char *filename, int isBW);
void save_quadtree_minimized(quadnode *tree, const char *filename, int isBW);

quadnode *load_quadtree(const char *filename, max_heap *heap, int isBW);
quadnode *load_quadtree_minimized(const char *filename, max_heap *heap, int isBW);

#endif
