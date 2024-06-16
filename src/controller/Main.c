#include "../../include/controller/Controller.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }
    int useCircles = argc >= 3 && strcmp(argv[2], "--circles") == 0;
    
    initialize_window();
    MLV_Image *image = load_image(argv[1]);
    quadnode *tree = NULL;

    int clickX, clickY;
    int buttonIndex = -1;
    char *pathInput = "";
    while (buttonIndex != 7) {
        draw_interface_buttons();
        if (tree != NULL) draw_quadtree(tree, useCircles);
        else draw_image(image);
        update_window();
        MLV_wait_mouse(&clickX, &clickY);
        hide_buttons();
        buttonIndex = clickX/128 + 4*((clickY-512)/50);
        if (buttonIndex != 0 && buttonIndex != 4 && tree == NULL) continue;
        switch (buttonIndex) {
            case 0:
                tree = process_image(image, useCircles);
                break;
            case 1:
                save_quadtree_unminimized(tree, "result.qtn", 1);
                break;
            case 2:
                save_quadtree_unminimized(tree, "result.qtc", 0);
                break;
            case 3:
                minimise_quadtree(tree);
                break;
            case 4:
                MLV_wait_input_box(0, 512, 512, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, "   Entrez un chemin valide : ", &pathInput);
                if (!MLV_path_is_a_file(pathInput)) break;
                free_quadnode(tree);
                int length = strlen(pathInput);
                if (strncmp(pathInput + length - 4, ".qtc", 4) == 0)
                    tree = load_quadtree(pathInput, 0);
                else if (strncmp(pathInput + length - 4, ".qtn", 4) == 0)
                    tree = load_quadtree(pathInput, 1);
                else if (strncmp(pathInput + length - 4, ".gmc", 4) == 0)
                    tree = load_minimised_quadtree(pathInput, 0);
                else if (strncmp(pathInput + length - 4, ".gmn", 4) == 0)
                    tree = load_minimised_quadtree(pathInput, 1);
                else {
                    tree = NULL;
                    MLV_free_image(image);
                    image = load_image(pathInput);
                }
                break;
            case 5:
                save_quadtree_minimized(tree, "result.gmn", 1);
                break;
            case 6:
                save_quadtree_minimized(tree, "result.gmc", 0);
                break;
        }
    }
    free_quadnode(tree);
    MLV_free_image(image);
    free_window();
    return 0;
}
