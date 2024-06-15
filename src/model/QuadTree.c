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

void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize) {
    *buffer <<= 1;
    *bufferSize += 1;
    if (node->northwest == NULL) {
        *buffer += 1;
        *buffer <<= 32;
        *bufferSize += 32;
        *buffer += ((long long) (node->color.red)%256) << 24;
        *buffer += ((long long) (node->color.blue)%256) << 16;
        *buffer += ((long long) (node->color.green)%256) << 8;
        *buffer += ((long long) (node->color.alpha)%256);
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
    FILE *fptr = fopen(filename, "w");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    write_node_to_file(tree, fptr, &buffer, &bufferSize);
    if (bufferSize != 0)
        fputc(buffer << (8-bufferSize), fptr);
    fclose(fptr);
}

void write_node_to_file_bw(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize) {
    *buffer <<= 1;
    *bufferSize += 1;
    if (node->northwest == NULL) {
        *buffer += 1;
        *buffer <<= 8;
        *bufferSize += 8;
        int grayscale = (node->color.red + node->color.green + node->color.blue) / 3;
        *buffer += grayscale % 256;
    }
    while (*bufferSize >= 8) {
        *bufferSize -= 8;
        char byte = *buffer >> *bufferSize;
        fputc(byte, fptr);
    }
    if (node->northwest != NULL) {
        write_node_to_file_bw(node->northwest, fptr, buffer, bufferSize);
        write_node_to_file_bw(node->northeast, fptr, buffer, bufferSize);
        write_node_to_file_bw(node->southwest, fptr, buffer, bufferSize);
        write_node_to_file_bw(node->southeast, fptr, buffer, bufferSize);
    }
}

void save_quadtree_bw(quadnode *tree, const char *filename) {
    FILE *fptr = fopen(filename, "w");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    write_node_to_file_bw(tree, fptr, &buffer, &bufferSize);
    if (bufferSize != 0)
        fputc(buffer << (8-bufferSize), fptr);
    fclose(fptr);
}

void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, max_heap *heap) {
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
        current.blue = (*buffer >> (*bufferSize - 16)) % 256;
        current.green = (*buffer >> (*bufferSize - 24)) % 256;
        current.alpha = (*buffer >> (*bufferSize - 32)) % 256;
        node->color = current;
        *bufferSize -= 32;
    } else {
        node->northwest = create_quadnode(NULL, node->x, node->y, node->size / 2, heap);
        node->northeast = create_quadnode(NULL, node->x + node->size / 2, node->y, node->size / 2, heap);
        node->southwest = create_quadnode(NULL, node->x, node->y + node->size / 2, node->size / 2, heap);
        node->southeast = create_quadnode(NULL, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
        read_node_from_file(node->northwest, fptr, buffer, bufferSize, heap);
        read_node_from_file(node->northeast, fptr, buffer, bufferSize, heap);
        read_node_from_file(node->southwest, fptr, buffer, bufferSize, heap);
        read_node_from_file(node->southeast, fptr, buffer, bufferSize, heap);
    }
}

void read_node_from_file_bw(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, max_heap *heap) {
    if (*bufferSize == 0) {
        *buffer = fgetc(fptr);
        *bufferSize += 8;
    }
    int is_leaf = (*buffer >> (*bufferSize - 1)) % 2;
    *bufferSize -= 1;
    if (is_leaf) {
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
        node->northwest = create_quadnode(NULL, node->x, node->y, node->size / 2, heap);
        node->northeast = create_quadnode(NULL, node->x + node->size / 2, node->y, node->size / 2, heap);
        node->southwest = create_quadnode(NULL, node->x, node->y + node->size / 2, node->size / 2, heap);
        node->southeast = create_quadnode(NULL, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
        read_node_from_file_bw(node->northwest, fptr, buffer, bufferSize, heap);
        read_node_from_file_bw(node->northeast, fptr, buffer, bufferSize, heap);
        read_node_from_file_bw(node->southwest, fptr, buffer, bufferSize, heap);
        read_node_from_file_bw(node->southeast, fptr, buffer, bufferSize, heap);
    }
}

quadnode *load_quadtree(const char *filename, max_heap *heap) {
    FILE *fptr = fopen(filename, "r");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, heap);
    read_node_from_file(tree, fptr, &buffer, &bufferSize, heap);
    fclose(fptr);
    return tree;
}

quadnode *load_quadtree_bw(const char *filename, max_heap *heap) {
    FILE *fptr = fopen(filename, "r");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, heap);
    read_node_from_file_bw(tree, fptr, &buffer, &bufferSize, heap);
    fclose(fptr);
    return tree;
}