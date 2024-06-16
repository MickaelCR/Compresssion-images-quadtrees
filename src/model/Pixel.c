#include "../../include/model/Pixel.h"
#include <MLV/MLV_all.h>
#include <math.h>

double distance(pixel pixel1, pixel pixel2) {
    // Calcul de la distance euclidienne entre les composantes RGBA des deux pixels
    return sqrt((pixel1.red - pixel2.red) * (pixel1.red - pixel2.red) +
                (pixel1.blue - pixel2.blue) * (pixel1.blue - pixel2.blue) +
                (pixel1.green - pixel2.green) * (pixel1.green - pixel2.green) +
                (pixel1.alpha - pixel2.alpha) * (pixel1.alpha - pixel2.alpha));
}

int equal_pixels(pixel pixel1, pixel pixel2) {
    // Comparaison des composantes RGBA des deux pixels
    return pixel1.red == pixel2.red && pixel1.blue == pixel2.blue && pixel1.green == pixel2.green && pixel1.alpha == pixel2.alpha;
}

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

MLV_Color to_MLV_Color(pixel pixel) {
    // Conversion des composantes RGBA en une couleur MLV
    return MLV_convert_rgba_to_color(pixel.red, pixel.green, pixel.blue, pixel.alpha);
}
