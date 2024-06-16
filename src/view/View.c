/**
 * @file View.c
 * @brief Fichier source pour la vue, gérant l'interface utilisateur et le rendu graphique.
 * 
 * Ce fichier contient les implémentations de fonctions pour la vue, qui gère l'interface utilisateur et le rendu graphique, y compris le chargement et l'affichage des images et des quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#include "../../include/view/View.h"
#include "../../include/model/QuadTree.h"
#include <MLV/MLV_all.h>

/**
 * @brief Initialise la fenêtre de l'application.
 */
void initialize_window() {
    MLV_create_window("Quadtree", "", 512, 612);
}

/**
 * @brief Dessine les boutons de l'interface utilisateur.
 */
void draw_interface_buttons() {
    // Dessine les boutons de l'interface utilisateur
    MLV_draw_filled_rectangle(0, 0, 512, 512, MLV_COLOR_WHITE);
    MLV_draw_text_box(0, 512, 128, 50, "Approximation", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(128, 512, 128, 50, "Sauv. Noir&Blanc", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(256, 512, 128, 50, "Sauv. RGBA", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(384, 512, 128, 50, "Minimisation", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(0, 562, 128, 50, "Charger Fichier", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(128, 562, 128, 50, "Sauv. Min. N&B", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(256, 562, 128, 50, "Sauv. Min. RGBA", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box(384, 562, 128, 50, "Quitter", 5, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_LIGHTGRAY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

/**
 * @brief Cache les boutons de l'interface utilisateur.
 */
void hide_buttons() {
    // Dessine un rectangle gris pour cacher les boutons
    MLV_draw_filled_rectangle(0, 512, 512, 100, MLV_COLOR_DARKGRAY);
    MLV_draw_rectangle(0, 512, 512, 100, MLV_COLOR_WHITE);
}

/**
 * @brief Charge une image depuis un fichier.
 * 
 * @param filename Le nom du fichier de l'image à charger.
 * @return Un pointeur vers l'image chargée.
 */
MLV_Image *load_image(const char *filename) {
    MLV_Image *image = MLV_load_image(filename);
    if (!image) {
        fprintf(stderr, "Could not load image %s\n", filename);
        return NULL;
    }
    return image;
}

/**
 * @brief Affiche une image dans la fenêtre.
 * 
 * @param image L'image à afficher.
 */
void draw_image(MLV_Image *image) {
    MLV_draw_image(image, 0, 0);
}

/**
 * @brief Dessine un quadtree dans la fenêtre.
 * 
 * @param tree Le quadtree à dessiner.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 */
void draw_quadtree(quadnode *tree, int useCircles) {
    if (tree->northwest == NULL) {
        // Le nœud est une feuille, dessine un cercle ou un rectangle selon l'option
        if (useCircles) {
            MLV_draw_filled_circle(tree->x + tree->size / 2, tree->y + tree->size / 2, tree->size / 2, to_MLV_Color(tree->color));
        } else {
            MLV_draw_filled_rectangle(tree->x, tree->y, tree->size, tree->size, to_MLV_Color(tree->color));
        }
        return;
    }
    // Appelle récursivement la fonction pour dessiner les sous-nœuds
    draw_quadtree(tree->northwest, useCircles);
    draw_quadtree(tree->northeast, useCircles);
    draw_quadtree(tree->southwest, useCircles);
    draw_quadtree(tree->southeast, useCircles);
}

/**
 * @brief Met à jour la fenêtre pour refléter les changements graphiques.
 */
void update_window() {
    MLV_actualise_window();
}

/**
 * @brief Libère les ressources associées à la fenêtre.
 */
void free_window() {
    MLV_free_window();
}
