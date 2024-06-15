#include "../../include/model/Pixel.h"
#include <MLV/MLV_all.h>
#include <math.h>

double distance(pixel pixel1, pixel pixel2) {
    return sqrt((pixel1.red - pixel2.red) * (pixel1.red - pixel2.red) +
                (pixel1.blue - pixel2.blue) * (pixel1.blue - pixel2.blue) +
                (pixel1.green - pixel2.green) * (pixel1.green - pixel2.green) +
                (pixel1.alpha - pixel2.alpha) * (pixel1.alpha - pixel2.alpha));
}

pixel read_image(MLV_Image *image, int startX, int startY, int size) {
    pixel current, final = {0, 0, 0, 0};
    int pixelCount = size * size;
    int x, y;
    for (x = startX; x < startX + size; x++) {
        for (y = startY; y < startY + size; y++) {
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            final.red += current.red, final.green += current.green, final.blue += current.blue, final.alpha += current.alpha;
        }
    }
    final.red /= pixelCount, final.green /= pixelCount, final.blue /= pixelCount, final.alpha /= pixelCount;
    return final;
}

MLV_Color to_MLV_Color(pixel pixel) {
	return MLV_convert_rgba_to_color(pixel.red, pixel.green, pixel.blue, pixel.alpha);
}