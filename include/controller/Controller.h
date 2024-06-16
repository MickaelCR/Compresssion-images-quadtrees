/**
 * @file Controller.h
 * @brief Fichier d'en-tête pour le contrôleur gérant les opérations sur les images et les quadtree.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour le contrôleur, qui gère le traitement des images et les opérations sur les quadtree, telles que la création, la subdivision, la minimisation et la sauvegarde des quadtree.
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

#endif // CONTROLLER_H
