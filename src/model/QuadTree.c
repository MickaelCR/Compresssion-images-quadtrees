#include "../../include/model/QuadTree.h"
#include <stdlib.h>

double calculate_error(MLV_Image *image, int startX, int startY, int size, pixel average) {
    double error = 0;
    pixel current;
    int x, y;
    for (x = startX; x < startX + size; x++) {
        for (y = startY; y < startY + size; y++) {
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            error += distance(average, current);
        }
    }
    return error;
}

quadnode *create_quadnode(MLV_Image *image, int x, int y, int size) {
    quadnode *node = malloc(sizeof(quadnode));
    node->color = read_image(image, x, y, size);
    node->error = calculate_error(image, x, y, size, node->color);
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    node->x = x;
    node->y = y;
    node->size = size;
    return node;
}

void subdivide(quadnode *node, MLV_Image *image) {
	node->northwest = create_quadnode(image, node->x, node->y, node->size/2);
	node->northeast = create_quadnode(image, node->x+node->size/2, node->y, node->size/2);
	node->southwest = create_quadnode(image, node->x, node->y+node->size/2, node->size/2);
	node->southeast = create_quadnode(image, node->x+node->size/2, node->y+node->size/2, node->size/2);
}

quadnode *find_max_error(quadnode *node) {
    if (node->northwest == NULL) return node;
    quadnode *current = find_max_error(node->northwest);
    quadnode *tmp = find_max_error(node->northeast);
    if (current->error < tmp->error) current = tmp;
    tmp = find_max_error(node->southwest);
    if (current->error < tmp->error) current = tmp;
    tmp = find_max_error(node->southeast);
    if (current->error < tmp->error) current = tmp;
    return current;
}
