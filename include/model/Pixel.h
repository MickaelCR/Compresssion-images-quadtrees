/**
 * @file pixel.h
 * @brief Fichier d'en-tête pour les opérations sur les pixels utilisés dans le projet QuadTree.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour les opérations sur les pixels, telles que le calcul de distance, la comparaison et la lecture des couleurs dans une image.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#ifndef MODEL_PIXEL_H
#define MODEL_PIXEL_H

#include <MLV/MLV_all.h>

/**
 * @brief Structure représentant un pixel avec des composantes RGBA.
 */
typedef struct _pixel_ {
    int red;    /**< Composante rouge du pixel. */
    int green;  /**< Composante verte du pixel. */
    int blue;   /**< Composante bleue du pixel. */
    int alpha;  /**< Composante alpha (transparence) du pixel. */
} pixel;

/**
 * @brief Calcule la distance entre deux pixels.
 * 
 * @param pixel1 Le premier pixel.
 * @param pixel2 Le deuxième pixel.
 * @return La distance entre les deux pixels.
 */
double distance(pixel pixel1, pixel pixel2);

/**
 * @brief Vérifie si deux pixels sont égaux.
 * 
 * @param pixel1 Le premier pixel.
 * @param pixel2 Le deuxième pixel.
 * @return 1 si les pixels sont égaux, 0 sinon.
 */
int equal_pixels(pixel pixel1, pixel pixel2);

/**
 * @brief Lit la couleur moyenne d'une région d'une image.
 * 
 * @param image L'image à partir de laquelle lire.
 * @param startX La coordonnée x de départ.
 * @param startY La coordonnée y de départ.
 * @param size La taille de la région à lire.
 * @return La couleur moyenne de la région spécifiée.
 */
pixel read_image(MLV_Image *image, int startX, int startY, int size);

/**
 * @brief Convertit une structure pixel en MLV_Color.
 * 
 * @param pixel Le pixel à convertir.
 * @return La couleur MLV correspondante.
 */
MLV_Color to_MLV_Color(pixel pixel);

#endif // MODEL_PIXEL_H
