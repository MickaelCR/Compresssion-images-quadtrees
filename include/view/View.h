#ifndef VIEW_H
#define VIEW_H

#include <MLV/MLV_all.h>
#include "../model/QuadTree.h"

void initialize_window();
void draw_interface_buttons();
MLV_Image *load_image(const char *filename);
void draw_image(MLV_Image *image);
void draw_quadtree(quadnode *tree);
void update_window();
void free_window();

#endif
