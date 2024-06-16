#include "../../include/model/Pixel.h"
#include <MLV/MLV_all.h>
#include <math.h>

/**
 * @brief Calcule la distance euclidienne entre deux pixels.
 * 
 * @param pixel1 Le premier pixel.
 * @param pixel2 Le second pixel.
 * @return La distance euclidienne entre les deux pixels.
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
 * @param pixel2 Le second pixel.
 * @return 1 si les pixels sont égaux, 0 sinon.
 */
int equal_pixels(pixel pixel1, pixel pixel2) {
    // Comparaison des composantes RGBA des deux pixels
    return pixel1.red == pixel2.red && pixel1.blue == pixel2.blue && pixel1.green == pixel2.green && pixel1.alpha == pixel2.alpha;
}

/**
 * @brief Lit les pixels d'une image et calcule la couleur moyenne d'un carré de taille spécifiée.
 * 
 * @param image L'image MLV à lire.
 * @param startX La coordonnée X de départ du carré.
 * @param startY La coordonnée Y de départ du carré.
 * @param size La taille du carré.
 * @return La couleur moyenne des pixels dans le carré spécifié.
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
 * @brief Convertit un pixel en une couleur MLV.
 * 
 * @param pixel Le pixel à convertir.
 * @return La couleur MLV correspondante.
 */
MLV_Color to_MLV_Color(pixel pixel) {
    // Conversion des composantes RGBA en une couleur MLV
    return MLV_convert_rgba_to_color(pixel.red, pixel.green, pixel.blue, pixel.alpha);
}
