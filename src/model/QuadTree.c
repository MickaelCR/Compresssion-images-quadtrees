#include "../../include/model/QuadTree.h"
#include <stdlib.h>

double calculate_error(MLV_Image *image, int startX, int startY, int width, int height, pixel average) {
    double error = 0;
    pixel current;
    int x, y;
    for (x = startX; x < startX + width; x++) {
        for (y = startY; y < startY + height; y++) {
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            error += distance(average, current);
        }
    }
    return error;
}

quadnode *create_quadnode(MLV_Image *image, int startX, int startY, int width, int height) {
    quadnode *node = malloc(sizeof(quadnode));
    node->color = read_image(image, startX, startY, width, height);
    node->error = calculate_error(image, startX, startY, width, height, node->color);
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    return node;
}
