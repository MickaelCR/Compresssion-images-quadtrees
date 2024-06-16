#include "../../include/model/QuadTree.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <stdlib.h>

double calculate_error(MLV_Image *image, int startX, int startY, int size, pixel average) {
    double error = 0;
    pixel current;
    int x, y;
    for (x = startX; x < startX + size; x++) {
        for (y = startY; y < startY + size; y++) {
            // Obtient les valeurs des pixels de l'image
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            // Calcule l'erreur par rapport à la couleur moyenne
            error += distance(average, current);
        }
    }
    return error;
}

quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap) {
    // Alloue de la mémoire pour un nouveau nœud
    quadnode *node = malloc(sizeof(quadnode));
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    node->x = x;
    node->y = y;
    node->size = size;
    node->id = -1; // Par défaut, aucun identifiant

    if (image != NULL) {
        // Lit la couleur moyenne de l'image pour cette région
        node->color = read_image(image, x, y, size);
        // Calcule l'erreur pour cette région
        node->error = calculate_error(image, x, y, size, node->color);
        // Ajouter le nœud au tas max pour le tri par erreur
        insert_max_heap(heap, node);
    }
    return node;
}

void subdivide(quadnode *node, MLV_Image *image, max_heap *heap) {
    if (node->size <= 1) return;

    // Subdivise le nœud en quatre quadrants plus petits
    node->northwest = create_quadnode(image, node->x, node->y, node->size / 2, heap);
    node->northeast = create_quadnode(image, node->x + node->size / 2, node->y, node->size / 2, heap);
    node->southwest = create_quadnode(image, node->x, node->y + node->size / 2, node->size / 2, heap);
    node->southeast = create_quadnode(image, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
}

void free_quadnode(quadnode *node) {
    if (node == NULL) return;
    // Libère la mémoire allouée pour chaque sous-nœud
    free_quadnode(node->northwest);
    free_quadnode(node->northeast);
    free_quadnode(node->southwest);
    free_quadnode(node->southeast);
    free(node);
}

void assign_ids(quadnode *node, int *current_id) {
    if (node == NULL || node->id != -1) return;
    // Assigne un identifiant unique au nœud
    node->id = (*current_id)++;
    // Assigne des identifiants aux sous-nœuds de manière récursive
    assign_ids(node->northwest, current_id);
    assign_ids(node->northeast, current_id);
    assign_ids(node->southwest, current_id);
    assign_ids(node->southeast, current_id);
}

void write_minimized_node_to_file(quadnode *node, FILE *fptr, int blackAndWhite) {
    if (node == NULL) return;
    if (node->northwest == NULL) {
        // Le nœud est une feuille
        if(blackAndWhite)
            fprintf(fptr, "%d %d\n", node->id, (((node->color.red + node->color.green + node->color.blue) / 3) / 128));
        else
            fprintf(fptr, "%df %d %d %d %d\n", node->id, node->color.red, node->color.green, node->color.blue, node->color.alpha);
    } else {
        // Le nœud est interne
        fprintf(fptr, "%d %d %d %d %d\n", node->id, node->northwest->id, node->northeast->id, node->southwest->id, node->southeast->id);
        // Écriture récursive des sous-nœuds
        write_minimized_node_to_file(node->northwest, fptr, blackAndWhite);
        write_minimized_node_to_file(node->northeast, fptr, blackAndWhite);
        write_minimized_node_to_file(node->southwest, fptr, blackAndWhite);
        write_minimized_node_to_file(node->southeast, fptr, blackAndWhite);
    }
}

void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, int isBW) {
    *buffer <<= 1;
    *bufferSize += 1;
    if (node->northwest == NULL) {
        *buffer += 1;
        if (isBW) {
            *buffer <<= 1;
            *bufferSize += 1;
            int grayscale = (node->color.red + node->color.green + node->color.blue) / 3;
            *buffer += (grayscale % 256) / 128;
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

void save_quadtree_unminimized(quadnode *tree, const char *filename, int isBW) {
    FILE *fptr = fopen(filename, "w");
    unsigned long long int buffer = 0;
    int bufferSize = 0;
    write_node_to_file(tree, fptr, &buffer, &bufferSize, isBW);
    if (bufferSize != 0)
        fputc(buffer << (8-bufferSize), fptr);
    fclose(fptr);
}

void save_quadtree_minimized(quadnode *tree, const char *filename, int isBW) {
    int current_id = 0;
    assign_ids(tree, &current_id);

    FILE *fptr = fopen(filename, "w");
    if (!fptr) {
        perror("Failed to open file for writing");
        return;
    }

    write_minimized_node_to_file(tree, fptr, isBW);
    fclose(fptr);
}

void read_node_from_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, int isBW) {
    if (*bufferSize == 0) {
        *buffer = fgetc(fptr);
        *bufferSize += 8;
    }
    int is_leaf = (*buffer >> (*bufferSize - 1)) % 2;
    *bufferSize -= 1;
    if (is_leaf) {
        if (isBW) {
            if (*bufferSize == 0) {
                *buffer <<= 8;
                *buffer += fgetc(fptr);
                *bufferSize += 8;
            }
            int grayscale = (*buffer >> (*bufferSize - 1)) % 2;
            pixel current;
            current.red = current.green = current.blue = grayscale * 255;
            current.alpha = 255;
            node->color = current;
            *bufferSize -= 1;
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
        node->northwest = create_quadnode(NULL, node->x, node->y, node->size / 2, NULL);
        node->northeast = create_quadnode(NULL, node->x + node->size / 2, node->y, node->size / 2, NULL);
        node->southwest = create_quadnode(NULL, node->x, node->y + node->size / 2, node->size / 2, NULL);
        node->southeast = create_quadnode(NULL, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, NULL);
        read_node_from_file(node->northwest, fptr, buffer, bufferSize, isBW);
        read_node_from_file(node->northeast, fptr, buffer, bufferSize, isBW);
        read_node_from_file(node->southwest, fptr, buffer, bufferSize, isBW);
        read_node_from_file(node->southeast, fptr, buffer, bufferSize, isBW);
    }
}

quadnode *load_quadtree(char *filename, int isBW) {
    // Ouvre le fichier en lecture
    FILE *fptr = fopen(filename, "r");
    if (!fptr) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    unsigned long long int buffer = 0;
    int bufferSize = 0;

    // Crée la racine de l'arbre quadtree
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, NULL);
    if (!tree) {
        fclose(fptr);
        return NULL;
    }

    // Lit les données du fichier et reconstruit l'arbre quadtree
    read_node_from_file(tree, fptr, &buffer, &bufferSize, isBW);

    // Ferme le fichier
    fclose(fptr);

    // Retourne la racine de l'arbre quadtree
    return tree;
}


int minimise_quadtree(quadnode *tree) {
    // Vérifie si l'arbre ou ses sous-nœuds sont vides
    if (tree == NULL || tree->northwest == NULL) return 0;

    // Appelle récursivement la fonction pour minimiser les sous-nœuds
    int sum = minimise_quadtree(tree->northwest) 
            + minimise_quadtree(tree->northeast) 
            + minimise_quadtree(tree->southwest) 
            + minimise_quadtree(tree->southeast);

    // Si tous les sous-nœuds peuvent être minimisés et ont la même couleur
    if (sum == 0 && equal_pixels(tree->northwest->color, tree->northeast->color) &&
                    equal_pixels(tree->northwest->color, tree->southwest->color) &&
                    equal_pixels(tree->northwest->color, tree->southeast->color)) {
        // Remplace la couleur du nœud courant par celle des sous-nœuds
        tree->color = tree->northwest->color;

        // Libère la mémoire allouée pour les sous-nœuds
        free_quadnode(tree->northwest);
        free_quadnode(tree->northeast);
        free_quadnode(tree->southwest);
        free_quadnode(tree->southeast);

        // Met à jour les pointeurs des sous-nœuds à NULL
        tree->northwest = NULL;
        tree->northeast = NULL;
        tree->southwest = NULL;
        tree->southeast = NULL;

        return 0; // Retourne 0 pour indiquer que le nœud courant a été minimisé
    }

    return 1; // Retourne 1 pour indiquer que le nœud courant ne peut pas être minimisé
}

void read_minimised_node_from_file(quadnode *node, FILE *fptr, int nodeIndex, int blackAndWhite) {
    char *line = NULL;
    size_t len;
    int currentIndex;
    char afterNumber;
    pixel currentPixel = {0, 0, 0, 0};

    while (1) {
        // Lit une ligne du fichier
        int lineValidity = getline(&line, &len, fptr);

        // Analyse la ligne lue et extrait les informations
        int scanned = sscanf(line, "%d%c%d %d %d %d", &currentIndex, &afterNumber, 
                             &(currentPixel.red), &(currentPixel.green), 
                             &(currentPixel.blue), &(currentPixel.alpha));

        // Si la ligne est valide et que l'indice actuel est supérieur à nodeIndex, revenir au début du fichier
        if (lineValidity != -1 && currentIndex > nodeIndex)
            rewind(fptr);

        // Si l'indice actuel correspond à nodeIndex, traiter le nœud
        if (currentIndex == nodeIndex) {
            // Si le nœud n'est pas en noir et blanc et que le caractère après le numéro est un espace
            // ou si le nœud est en noir et blanc et qu'il y a 6 valeurs scannées
            if ((!blackAndWhite && afterNumber == ' ') || (blackAndWhite && scanned == 6)) {
                // Crée des sous-nœuds pour le nœud courant
                node->northwest = create_quadnode(NULL, node->x, node->y, node->size/2, NULL);
                node->northeast = create_quadnode(NULL, node->x + node->size/2, node->y, node->size/2, NULL);
                node->southwest = create_quadnode(NULL, node->x, node->y + node->size/2, node->size/2, NULL);
                node->southeast = create_quadnode(NULL, node->x + node->size/2, node->y + node->size/2, node->size/2, NULL);

                // Lire récursivement les sous-nœuds
                read_minimised_node_from_file(node->northwest, fptr, currentPixel.red, blackAndWhite);
                read_minimised_node_from_file(node->northeast, fptr, currentPixel.green, blackAndWhite);
                read_minimised_node_from_file(node->southwest, fptr, currentPixel.blue, blackAndWhite);
                read_minimised_node_from_file(node->southeast, fptr, currentPixel.alpha, blackAndWhite);
            } 
            // Si le nœud n'est pas en noir et blanc et que le caractère après le numéro est 'f'
            else if (!blackAndWhite && afterNumber == 'f') {
                node->color = currentPixel;  // Assigner la couleur courante au nœud
            } 
            // Si le nœud est en noir et blanc et qu'il y a 3 valeurs scannées
            else if (blackAndWhite && scanned == 3) {
                currentPixel.red *= 255;
                currentPixel.green = currentPixel.red;
                currentPixel.blue = currentPixel.red;
                currentPixel.alpha = 255;
                node->color = currentPixel;  // Assigner la couleur en niveaux de gris au nœud
            }
            break;  // Sortir de la boucle une fois le nœud traité
        }
    }
}


quadnode *load_minimised_quadtree(char *filename, int blackAndWhite) {
    FILE *fptr = fopen(filename, "r");
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, NULL);
    read_minimised_node_from_file(tree, fptr, 0, blackAndWhite);
    fclose(fptr);
    return tree;
}
