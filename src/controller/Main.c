#include "../../include/controller/Controller.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }
    
    initialize_window();
    draw_interface_buttons();
    MLV_Image *image = load_image(argv[1]);
    quadnode *tree = NULL;

    int clickX, clickY;
    int buttonIndex = -1;
    while (buttonIndex != 7) {
        if (tree != NULL) draw_quadtree(tree);
        else draw_image(image);
        update_window();
        MLV_wait_mouse(&clickX, &clickY);
        buttonIndex = clickX/128 + 4*((clickY-512)/50);
        switch (buttonIndex) {
            case 0:
                tree = process_image(image);
                break;
        }
    }
    MLV_free_image(image);
    free_window();
    return 0;
}
