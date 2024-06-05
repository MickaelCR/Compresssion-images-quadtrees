#include "../../include/model/Pixel.h"
#include <MLV/MLV_all.h>
#include <math.h>

double distance(pixel pixel1, pixel pixel2) {
    return sqrt((pixel1.red - pixel2.red) * (pixel1.red - pixel2.red) +
                (pixel1.blue - pixel2.blue) * (pixel1.blue - pixel2.blue) +
                (pixel1.green - pixel2.green) * (pixel1.green - pixel2.green) +
                (pixel1.alpha - pixel2.alpha) * (pixel1.alpha - pixel2.alpha));
}

pixel read_image(MLV_Image *image, int startX, int startY, int width, int height) {
    pixel current, final = {0, 0, 0, 0};
    int pixelCount = width * height;
    int x, y;
    for (x = startX; x < startX + width; x++) {
        for (y = startY; y < startY + height; y++) {
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            final.red += current.red, final.green += current.green, final.blue += current.blue, final.alpha += current.alpha;
        }
    }
    final.red /= pixelCount, final.green /= pixelCount, final.blue /= pixelCount, final.alpha /= pixelCount;
    return final;
}
