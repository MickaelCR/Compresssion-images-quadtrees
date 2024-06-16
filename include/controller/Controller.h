/**
 * @file Controller.h
 * @brief Fichier d'en-tête pour le contrôleur gérant les opérations sur les images et les quadtree.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour le contrôleur, 
 * qui gère le traitement des images et les opérations sur les quadtree, 
 * telles que la création, la subdivision, la minimisation et la sauvegarde des quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <MLV/MLV_all.h>
#include "../model/QuadTree.h"
#include "../view/View.h"

/**
 * @brief Traite une image et génère un quadtree.
 * 
 * @param image L'image à traiter.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 * @return Pointeur vers le quadtree généré.
 */
quadnode *process_image(MLV_Image *image, int useCircles);

/**
 * @brief Gère les clics sur les boutons de l'interface.
 * 
 * Cette fonction traite les clics sur les boutons de l'interface et effectue les actions appropriées,
 * telles que l'approximation de l'image, la sauvegarde et le chargement des quadtree, 
 * ainsi que la minimisation du quadtree.
 * 
 * @param buttonIndex Index du bouton cliqué.
 * @param image Pointeur vers l'image à traiter.
 * @param tree Pointeur vers le quadtree.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 * @param pathInput Chemin d'accès pour charger/sauvegarder les fichiers.
 */
void handle_button_click(int buttonIndex, MLV_Image **image, quadnode **tree, int useCircles, char **pathInput);

/**
 * @brief Boucle principale de l'application.
 * 
 * Cette fonction gère la boucle principale de l'application, 
 * en attendant les clics de l'utilisateur et en appelant les fonctions appropriées 
 * pour traiter les images et gérer les quadtree.
 * 
 * @param image Pointeur vers l'image à traiter.
 * @param tree Pointeur vers le quadtree.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 */
void main_loop(MLV_Image *image, quadnode *tree, int useCircles);

/**
 * @brief Traite le clic sur un bouton de l'interface.
 * 
 * Cette fonction est appelée lorsqu'un bouton de l'interface est cliqué. 
 * Elle effectue l'action correspondante en fonction de l'index du bouton.
 * 
 * @param buttonIndex Index du bouton cliqué.
 * @param image Pointeur vers l'image à traiter.
 * @param tree Pointeur vers le quadtree.
 * @param useCircles Indique si des cercles doivent être utilisés pour dessiner les nœuds.
 * @param pathInput Chemin d'accès pour charger/sauvegarder les fichiers.
 */
void process_button_click(int buttonIndex, MLV_Image **image, quadnode **tree, int useCircles, char **pathInput);

#endif // CONTROLLER_H
