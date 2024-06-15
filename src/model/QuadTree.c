#include "../../include/model/QuadTree.h"
#include "../../include/model/Heap.h"
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

quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap) {
    quadnode *node = malloc(sizeof(quadnode));
    node->color = read_image(image, x, y, size);
    node->error = calculate_error(image, x, y, size, node->color);
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    node->x = x;
    node->y = y;
    node->size = size;

    // Ajouter le nœud au tas max
    insert_max_heap(heap, node);

    return node;
}

void subdivide(quadnode *node, MLV_Image *image, max_heap *heap) {
    if (node->size <= 1) return; // Ne subdivisez pas si le nœud est de taille 1x1

    node->northwest = create_quadnode(image, node->x, node->y, node->size / 2, heap);
    node->northeast = create_quadnode(image, node->x + node->size / 2, node->y, node->size / 2, heap);
    node->southwest = create_quadnode(image, node->x, node->y + node->size / 2, node->size / 2, heap);
    node->southeast = create_quadnode(image, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
}
void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize) {
    *buffer <<= 1;
    *bufferSize += 1;
    if (node->northwest == NULL) {
        *buffer += 1;
        *buffer <<= 32;
        *bufferSize += 32;
        *buffer += ((unsigned long long) (node->color.red) % 256) << 24;
        *buffer += ((unsigned long long) (node->color.green) % 256) << 16;
        *buffer += ((unsigned long long) (node->color.blue) % 256) << 8;
        *buffer += ((unsigned long long) (node->color.alpha) % 256);
    }
    while (*bufferSize >= 8) {
        *bufferSize -= 8;
        char byte = *buffer >> *bufferSize;
        fputc(byte, fptr);
    }
    if (node->northwest != NULL) {
        write_node_to_file(node->northwest, fptr, buffer, bufferSize);
        write_node_to_file(node->northeast, fptr, buffer, bufferSize);
        write_node_to_file(node->southwest, fptr, buffer, bufferSize);
        write_node_to_file(node->southeast, fptr, buffer, bufferSize);
    }
}

void save_quadtree(quadnode *tree, const char *filename) {
    FILE *fptr = fopen(filename, "wb");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    write_node_to_file(tree, fptr, &buffer, &bufferSize);
    if (bufferSize != 0)
        fputc(buffer << (8 - bufferSize), fptr);
    fclose(fptr);
}

quadnode *create_empty_quadnode(int x, int y, int size) {
    quadnode *node = malloc(sizeof(quadnode));
    node->color = (pixel) {0, 0, 0, 0};
    node->error = 0;
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    node->x = x;
    node->y = y;
    node->size = size;
    return node;
}

void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize) {
    if (*bufferSize == 0) {
        *buffer = fgetc(fptr);
        *bufferSize += 8;
    }
    int is_leaf = (*buffer >> (*bufferSize - 1)) % 2;
    *bufferSize -= 1;
    if (is_leaf) {
        for (int i = 0; i < 4; i++) {
            *buffer <<= 8;
            *buffer += fgetc(fptr);
            *bufferSize += 8;
        }
        pixel current;
        current.red = (*buffer >> (*bufferSize - 8)) % 256;
        current.green = (*buffer >> (*bufferSize - 16)) % 256;
        current.blue = (*buffer >> (*bufferSize - 24)) % 256;
        current.alpha = (*buffer >> (*bufferSize - 32)) % 256;
        node->color = current;
        *bufferSize -= 32;
    } else {
        node->northwest = create_empty_quadnode(node->x, node->y, node->size / 2);
        node->northeast = create_empty_quadnode(node->x + node->size / 2, node->y, node->size / 2);
        node->southwest = create_empty_quadnode(node->x, node->y + node->size / 2, node->size / 2);
        node->southeast = create_empty_quadnode(node->x + node->size / 2, node->y + node->size / 2, node->size / 2);

        read_node_from_file(node->northwest, fptr, buffer, bufferSize);
        read_node_from_file(node->northeast, fptr, buffer, bufferSize);
        read_node_from_file(node->southwest, fptr, buffer, bufferSize);
        read_node_from_file(node->southeast, fptr, buffer, bufferSize);
    }
}

quadnode *load_quadtree(const char *filename) {
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    quadnode *tree = create_empty_quadnode(0, 0, 512);
    read_node_from_file(tree, fptr, &buffer, &bufferSize);
    fclose(fptr);
    return tree;
}
