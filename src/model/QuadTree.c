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
    if (image != NULL) {
        node->color = read_image(image, x, y, size);
        node->error = calculate_error(image, x, y, size, node->color);
    }
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

void free_quadnode(quadnode *node) {
    if (node == NULL) return;
    free_quadnode(node->northwest);
    free_quadnode(node->northeast);
    free_quadnode(node->southwest);
    free_quadnode(node->southeast);
    free(node);
}

void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, int isBW) {
    *buffer <<= 1;
    *bufferSize += 1;
    if (node->northwest == NULL) {
        *buffer += 1;
        if (isBW) {
            *buffer <<= 8;
            *bufferSize += 8;
            int grayscale = (node->color.red + node->color.green + node->color.blue) / 3;
            *buffer += grayscale % 256;
        } else {
            *buffer <<= 32;
            *bufferSize += 32;
            *buffer += ((long long) (node->color.red) % 256) << 24;
            *buffer += ((long long) (node->color.blue) % 256) << 16;
            *buffer += ((long long) (node->color.green) % 256) << 8;
            *buffer += ((long long) (node->color.alpha) % 256);
        }
    }
    while (*bufferSize >= 8) {
        *bufferSize -= 8;
        char byte = *buffer >> *bufferSize;
        fputc(byte, fptr);
    }
    if (node->northwest != NULL) {
        write_node_to_file(node->northwest, fptr, buffer, bufferSize, isBW);
        write_node_to_file(node->northeast, fptr, buffer, bufferSize, isBW);
        write_node_to_file(node->southwest, fptr, buffer, bufferSize, isBW);
        write_node_to_file(node->southeast, fptr, buffer, bufferSize, isBW);
    }
}

void save_quadtree(quadnode *tree, const char *filename, int isBW) {
    FILE *fptr = fopen(filename, "w");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    write_node_to_file(tree, fptr, &buffer, &bufferSize, isBW);
    if (bufferSize != 0)
        fputc(buffer << (8-bufferSize), fptr);
    fclose(fptr);
}

void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, max_heap *heap, int isBW) {
    if (*bufferSize == 0) {
        *buffer = fgetc(fptr);
        *bufferSize += 8;
    }
    int is_leaf = (*buffer >> (*bufferSize - 1)) % 2;
    *bufferSize -= 1;
    if (is_leaf) {
        if (isBW) {
            *buffer <<= 8;
            *buffer += fgetc(fptr);
            *bufferSize += 8;
            int grayscale = (*buffer >> (*bufferSize - 8)) % 256;
            pixel current;
            current.red = current.green = current.blue = grayscale;
            current.alpha = 255; // assuming full opacity
            node->color = current;
            *bufferSize -= 8;
        } else {
            for (int i = 0; i < 4; i++) {
                *buffer <<= 8;
                *buffer += fgetc(fptr);
                *bufferSize += 8;
            }
            pixel current;
            current.red = (*buffer >> (*bufferSize - 8)) % 256;
            current.blue = (*buffer >> (*bufferSize - 16)) % 256;
            current.green = (*buffer >> (*bufferSize - 24)) % 256;
            current.alpha = (*buffer >> (*bufferSize - 32)) % 256;
            node->color = current;
            *bufferSize -= 32;
        }
    } else {
        node->northwest = create_quadnode(NULL, node->x, node->y, node->size / 2, heap);
        node->northeast = create_quadnode(NULL, node->x + node->size / 2, node->y, node->size / 2, heap);
        node->southwest = create_quadnode(NULL, node->x, node->y + node->size / 2, node->size / 2, heap);
        node->southeast = create_quadnode(NULL, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
        read_node_from_file(node->northwest, fptr, buffer, bufferSize, heap, isBW);
        read_node_from_file(node->northeast, fptr, buffer, bufferSize, heap, isBW);
        read_node_from_file(node->southwest, fptr, buffer, bufferSize, heap, isBW);
        read_node_from_file(node->southeast, fptr, buffer, bufferSize, heap, isBW);
    }
}

quadnode *load_quadtree(const char *filename, max_heap *heap, int isBW) {
    FILE *fptr = fopen(filename, "r");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, heap);
    read_node_from_file(tree, fptr, &buffer, &bufferSize, heap, isBW);
    fclose(fptr);
    return tree;
}

void minimise_quadtree(quadnode *tree) {
    if (tree == NULL || tree->northwest == NULL) return;
    minimise_quadtree(tree->northwest);
    minimise_quadtree(tree->northeast);
    minimise_quadtree(tree->southwest);
    minimise_quadtree(tree->southeast);
    if (equal_pixels(tree->northwest->color, tree->northeast->color) &&
        equal_pixels(tree->northwest->color, tree->southwest->color) &&
        equal_pixels(tree->northwest->color, tree->southeast->color)) {
        free_quadnode(tree->northwest);
        free_quadnode(tree->northeast);
        free_quadnode(tree->southwest);
        free_quadnode(tree->southeast);
        tree->northwest = NULL;
        tree->northeast = NULL;
        tree->southwest = NULL;
        tree->southeast = NULL;
    }
}