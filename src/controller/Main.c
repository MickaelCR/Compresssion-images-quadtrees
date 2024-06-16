#include "../../include/controller/Controller.h"
#include <getopt.h>

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [--circles] <image_file>\n", argv[0]);
        return 1;
    }

    static struct option long_options[] = {
        {"circles", no_argument, NULL, 'c'},
        {NULL, 0, NULL, 0}
    };

    int useCircles = 0;
    int option_index = 0;
    int opt;

    while ((opt = getopt_long(argc, argv, "c", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':
                useCircles = 1;
                break;  
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [--circles] <image_file>\n", argv[0]);
        return 1;
    }

    const char *image_file = argv[optind];

    // Initialisation de la fenêtre
    initialize_window();
    MLV_Image *image = load_image(image_file);
    quadnode *tree = NULL;

    // Boucle principale
    main_loop(image, tree, useCircles);

    MLV_free_image(image);
    free_window();
    return 0;
}
