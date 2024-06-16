/**
 * @file Controller.c
 * @brief Implémentation des fonctions pour le contrôleur gérant les opérations sur les images et les quadtree.
 * 
 * Ce fichier contient les fonctions pour le traitement des images et les opérations sur les quadtree, 
 * telles que la création, la subdivision, la minimisation et la sauvegarde des quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#include "../../include/controller/Controller.h"
#include "../../include/view/View.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Traite une image et génère un quadtree.
 * 
 * @param image L'image à traiter.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 * @return Pointeur vers le quadtree généré.
 */
quadnode *process_image(MLV_Image *image, int useCircles) {
    // Création d'un tas max
    max_heap *heap = create_max_heap(1000);
    // Création de la racine de l'arbre quadtree
    quadnode *tree = create_quadnode(image, 0, 0, 512, heap);
    int multiple = 1;

    // Boucle pour subdiviser les noeuds
    for (int i = 0; i < 10000; i++) {
        quadnode *max_error_node = extract_max(heap);
        // Vérifie si le noeud a une taille supérieure à 1 pour continuer la subdivision
        if (max_error_node && max_error_node->size > 1) {
            subdivide_quadnode(max_error_node, image, heap);
        }
        // Mise à jour et affichage de l'arbre à des intervalles exponentiels
        if (multiple == i) {
            multiple *= 2;
            MLV_draw_filled_rectangle(0, 0, 512, 512, MLV_COLOR_WHITE);
            draw_quadtree(tree, useCircles);
            update_window();
            sleep(1);
        }
    }
    free_max_heap(heap);
    return tree;
}

/**
 * @brief Charge un quadtree ou une image à partir d'un chemin de fichier.
 * 
 * @param pathInput Chemin d'accès du fichier à charger.
 * @param image Pointeur vers l'image à mettre à jour.
 * @param tree Pointeur vers le quadtree à mettre à jour.
 */
void load_tree(char **pathInput, MLV_Image **image, quadnode **tree) {
    // Boîte de dialogue pour entrer un chemin de fichier valide
    MLV_wait_input_box(0, 512, 512, 100, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, "   Entrez un chemin valide : ", pathInput);
    if (!MLV_path_is_a_file(*pathInput)) return; // Vérifie si le chemin est valide
    free_quadnode(*tree);
    int length = strlen(*pathInput);

    // Charge le quadtree en fonction de l'extension du fichier
    if (strncmp(*pathInput + length - 4, ".qtc", 4) == 0)
        *tree = load_unminimized_quadtree(*pathInput, 0);
    else if (strncmp(*pathInput + length - 4, ".qtn", 4) == 0)
        *tree = load_unminimized_quadtree(*pathInput, 1);
    else if (strncmp(*pathInput + length - 4, ".gmc", 4) == 0)
        *tree = load_minimized_quadtree(*pathInput, 0);
    else if (strncmp(*pathInput + length - 4, ".gmn", 4) == 0)
        *tree = load_minimized_quadtree(*pathInput, 1);
    else {
        *tree = NULL;
        MLV_free_image(*image);
        *image = load_image(*pathInput);
    }
}

/**
 * @brief Gère les clics sur les boutons de l'interface.
 * 
 * @param buttonIndex Index du bouton cliqué.
 * @param image Pointeur vers l'image à traiter.
 * @param tree Pointeur vers le quadtree.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 * @param pathInput Chemin d'accès pour charger/sauvegarder les fichiers.
 */
void handle_button_click(int buttonIndex, MLV_Image **image, quadnode **tree, int useCircles, char **pathInput) {
    // Gère les clics sur les boutons de l'interface
    switch (buttonIndex) {
        case 0:
            *tree = process_image(*image, useCircles);
            break;
        case 1:
            save_unminimized_quadtree(*tree, "resources/save/result.qtn", 1);
            break;
        case 2:
            save_unminimized_quadtree(*tree, "resources/save/result.qtc", 0);
            break;
        case 3:
            minimise_quadtree(*tree);
            break;
        case 4:
            load_tree(pathInput, image, tree);
            break;
        case 5:
            save_minimized_quadtree(*tree, "resources/save/result.gmn", 1);
            break;
        case 6:
            minimise_quadtree(*tree);
            break;
    }
}

/**
 * @brief Boucle principale de l'application.
 * 
 * @param image Pointeur vers l'image à traiter.
 * @param tree Pointeur vers le quadtree.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 */
void main_loop(MLV_Image *image, quadnode *tree, int useCircles) {
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

        buttonIndex = clickX / 128 + 4 * ((clickY - 512) / 50);
        if (buttonIndex != 0 && buttonIndex != 4 && tree == NULL) continue;
        
        handle_button_click(buttonIndex, &image, &tree, useCircles, &pathInput);
    }

    free_quadnode(tree);
}
