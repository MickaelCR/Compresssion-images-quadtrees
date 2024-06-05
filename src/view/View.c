#include "../../include/view/View.h"

void initialize_window() {
    MLV_create_window("Quadtree", "", 512, 512);
}

void draw_image(MLV_Image *image) {
    MLV_draw_image(image, 0, 0);
}

void update_window() {
    MLV_actualise_window();
}

void free_window() {
    MLV_free_window();
}
