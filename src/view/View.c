#include "../../include/view/View.h"
#include "../../include/model/QuadTree.h"

#include <MLV/MLV_all.h>

void initialize_window() {
    MLV_create_window("Quadtree", "", 512, 612);
}

void draw_interface_buttons() {
	MLV_draw_filled_rectangle(0, 0, 512, 512, MLV_COLOR_WHITE);
	MLV_draw_text_box(0, 512, 128, 50, "Approximation", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(128, 512, 128, 50, "Sauv. Noir&Blanc", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(256, 512, 128, 50, "Sauv. Couleur", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(384, 512, 128, 50, "Minimisation", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(0, 562, 128, 50, "Charger Fichier", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(128, 562, 128, 50, "Sauv. Min. N&B", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(256, 562, 128, 50, "Sauv. Min. Couleur", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
	MLV_draw_text_box(384, 562, 128, 50, "Quitter", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

MLV_Image *load_image(const char *filename) {
	MLV_Image *image = MLV_load_image(filename);
    if (!image) {
        fprintf(stderr, "Could not load image %s\n", filename);
        return NULL;
    }
    return image;
}

void draw_image(MLV_Image *image) {
    MLV_draw_image(image, 0, 0);
}

void draw_quadtree(quadnode *tree, int useCircles) {
	if (tree->northwest == NULL) {
		if (useCircles) MLV_draw_filled_circle(tree->x+tree->size/2, tree->y+tree->size/2, tree->size/2, to_MLV_Color(tree->color));
		else MLV_draw_filled_rectangle(tree->x, tree->y, tree->size, tree->size, to_MLV_Color(tree->color));
		return;
	}
	draw_quadtree(tree->northwest, useCircles);
	draw_quadtree(tree->northeast, useCircles);
	draw_quadtree(tree->southwest, useCircles);
	draw_quadtree(tree->southeast, useCircles);
}

void update_window() {
    MLV_actualise_window();
}

void free_window() {
    MLV_free_window();
}
