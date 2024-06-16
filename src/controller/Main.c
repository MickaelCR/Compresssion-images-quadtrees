#include "../../include/controller/Controller.h"

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }
    int useCircles = argc >= 3 && strcmp(argv[2], "--circles") == 0;

    // Initialisation de la fenêtre
    initialize_window();
    MLV_Image *image = load_image(argv[1]);
    quadnode *tree = NULL;

    // Boucle principale
    main_loop(image, tree, useCircles);

    // Libération des ressources
    MLV_free_image(image);
    free_window();
    return 0;
}
