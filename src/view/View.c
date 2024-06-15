#include "../../include/view/View.h"
#include "../../include/model/QuadTree.h"

#include <MLV/MLV_all.h>

void initialize_window() {
    MLV_create_window("Quadtree", "", 512, 612);
}

void draw_image(MLV_Image *image) {
    MLV_draw_image(image, 0, 0);
}

void draw_quadtree(quadnode *tree) {
	if (tree->northwest == NULL) {
		MLV_draw_filled_rectangle(tree->x, tree->y, tree->size, tree->size, to_MLV_Color(tree->color));
		update_window();
		return;
	}
	draw_quadtree(tree->northwest);
	draw_quadtree(tree->northeast);
	draw_quadtree(tree->southwest);
	draw_quadtree(tree->southeast);
}

void update_window() {
    MLV_actualise_window();
}

void free_window() {
    MLV_free_window();
}
