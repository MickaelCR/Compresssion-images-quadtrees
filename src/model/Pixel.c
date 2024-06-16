/**
 * @file Pixel.c
 * @brief Fichier source pour les opérations sur les pixels utilisés dans le projet QuadTree.
 * 
 * Ce fichier contient les définitions des fonctions pour les opérations sur les pixels, telles que le calcul de distance, la comparaison et la lecture des couleurs dans une image.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#include "../../include/model/Pixel.h"
#include <MLV/MLV_all.h>
#include <math.h>

/**
 * @brief Calcule la distance entre deux pixels.
 * 
 * @param pixel1 Le premier pixel.
 * @param pixel2 Le deuxième pixel.
 * @return La distance entre les deux pixels.
 */
double distance(pixel pixel1, pixel pixel2) {
    // Calcul de la distance euclidienne entre les composantes RGBA des deux pixels
    return sqrt((pixel1.red - pixel2.red) * (pixel1.red - pixel2.red) +
                (pixel1.blue - pixel2.blue) * (pixel1.blue - pixel2.blue) +
                (pixel1.green - pixel2.green) * (pixel1.green - pixel2.green) +
                (pixel1.alpha - pixel2.alpha) * (pixel1.alpha - pixel2.alpha));
}

/**
 * @brief Vérifie si deux pixels sont égaux.
 * 
 * @param pixel1 Le premier pixel.
 * @param pixel2 Le deuxième pixel.
 * @return 1 si les pixels sont égaux, 0 sinon.
 */
int equal_pixels(pixel pixel1, pixel pixel2) {
    // Comparaison des composantes RGBA des deux pixels
    return pixel1.red == pixel2.red && pixel1.blue == pixel2.blue && pixel1.green == pixel2.green && pixel1.alpha == pixel2.alpha;
}

/**
 * @brief Lit la couleur moyenne d'une région d'une image.
 * 
 * @param image L'image à partir de laquelle lire.
 * @param startX La coordonnée x de départ.
 * @param startY La coordonnée y de départ.
 * @param size La taille de la région à lire.
 * @return La couleur moyenne de la région spécifiée.
 */
pixel read_image(MLV_Image *image, int startX, int startY, int size) {
    pixel current, final = {0, 0, 0, 0};  // Initialisation des pixels courant et final
    int pixelCount = size * size;  // Nombre total de pixels dans le carré
    int x, y;
    for (x = startX; x < startX + size; x++) {
        for (y = startY; y < startY + size; y++) {
            // Lecture des composantes RGBA du pixel courant
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            // Addition des valeurs des composantes pour calculer la moyenne
            final.red += current.red;
            final.green += current.green;
            final.blue += current.blue;
            final.alpha += current.alpha;
        }
    }
    // Calcul de la couleur moyenne
    final.red /= pixelCount;
    final.green /= pixelCount;
    final.blue /= pixelCount;
    final.alpha /= pixelCount;
    return final;  // Retourne la couleur moyenne
}

/**
 * @brief Convertit une structure pixel en MLV_Color.
 * 
 * @param pixel Le pixel à convertir.
 * @return La couleur MLV correspondante.
 */
MLV_Color to_MLV_Color(pixel pixel) {
    // Conversion des composantes RGBA en une couleur MLV
    return MLV_convert_rgba_to_color(pixel.red, pixel.green, pixel.blue, pixel.alpha);
}
