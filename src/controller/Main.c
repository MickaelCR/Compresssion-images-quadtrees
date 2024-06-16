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

    int clickX, clickY;
    int buttonIndex = -1;
    char *pathInput = "";

    // Boucle principale
    while (buttonIndex != 7) {
        draw_interface_buttons();
        if (tree != NULL) draw_quadtree(tree, useCircles);
        else draw_image(image);
        update_window();

        // Attente du clic de souris
        MLV_wait_mouse(&clickX, &clickY);
        hide_buttons();

        // Calcul de l'index du bouton cliqué
        buttonIndex = clickX / 128 + 4 * ((clickY - 512) / 50);
        printf("%d\n", buttonIndex);

        // Ignorer les clics sur les boutons non valides
        if (buttonIndex != 0 && buttonIndex != 4 && tree == NULL) continue;

        // Gérer les clics sur les boutons
        switch (buttonIndex) {
            case 0:
                tree = process_image(image, useCircles);
                break;
            case 1:
                save_unminimized_quadtree(tree, "result.qtn", 1);
                break;
            case 2:
                save_unminimized_quadtree(tree, "result.qtc", 0);
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
                    tree = load_unminimized_quadtree(pathInput, 0);
                else if (strncmp(pathInput + length - 4, ".qtn", 4) == 0)
                    tree = load_unminimized_quadtree(pathInput, 1);
                else if (strncmp(pathInput + length - 4, ".gmc", 4) == 0)
                    tree = load_minimized_quadtree(pathInput, 0);
                else if (strncmp(pathInput + length - 4, ".gmn", 4) == 0)
                    tree = load_minimized_quadtree(pathInput, 1);
                else {
                    tree = NULL;
                    MLV_free_image(image);
                    image = load_image(pathInput);
                }
                break;
            case 5:
                save_minimized_quadtree(tree, "result.gmn", 1);
                break;
            case 6:
                save_minimized_quadtree(tree, "result.gmc", 0);
                break;
        }
    }

    // Libération des ressources
    free_quadnode(tree);
    MLV_free_image(image);
    free_window();
    return 0;
}
