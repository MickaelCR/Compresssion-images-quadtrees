/**
 * @file View.h
 * @brief Fichier d'en-tête pour la vue, gérant l'interface utilisateur et le rendu graphique.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour la vue, qui gère l'interface utilisateur et le rendu graphique, y compris le chargement et l'affichage des images et des quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#ifndef VIEW_H
#define VIEW_H

#include <MLV/MLV_all.h>
#include "../model/QuadTree.h"

/**
 * @brief Initialise la fenêtre de l'application.
 */
void initialize_window();

/**
 * @brief Dessine les boutons de l'interface utilisateur.
 */
void draw_interface_buttons();

/**
 * @brief Cache les boutons de l'interface utilisateur.
 */
void hide_buttons();

/**
 * @brief Charge une image depuis un fichier.
 * 
 * @param filename Le nom du fichier de l'image à charger.
 * @return Un pointeur vers l'image chargée.
 */
MLV_Image *load_image(const char *filename);

/**
 * @brief Affiche une image dans la fenêtre.
 * 
 * @param image L'image à afficher.
 */
void draw_image(MLV_Image *image);

/**
 * @brief Dessine un quadtree dans la fenêtre.
 * 
 * @param tree Le quadtree à dessiner.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 */
void draw_quadtree(quadnode *tree, int useCircles);

/**
 * @brief Met à jour la fenêtre pour refléter les changements graphiques.
 */
void update_window();

/**
 * @brief Libère les ressources associées à la fenêtre.
 */
void free_window();

#endif // VIEW_H
