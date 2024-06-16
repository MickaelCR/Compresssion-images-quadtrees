#include <stdio.h>
#include <stdlib.h>
#include "../../include/model/QuadTree.h"
#include "../../include/model/Heap.h"

double calculate_error(MLV_Image *image, int startX, int startY, int size, pixel average) {
    double error = 0;
    pixel current;
    for (int x = startX; x < startX + size; x++) {
        for (int y = startY; y < startY + size; y++) {
            // Obtient la couleur actuelle du pixel à la position (x, y)
            MLV_get_pixel_on_image(image, x, y, &(current.red), &(current.green), &(current.blue), &(current.alpha));
            // Calcule l'erreur en comparant la couleur actuelle avec la couleur moyenne
            error += distance(average, current);
        }
    }
    return error;
}

quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap) {
    // Alloue de la mémoire pour un nouveau nœud quadtree
    quadnode *node = malloc(sizeof(quadnode));
    if (!node) {
        perror("Failed to allocate memory for quadnode");
        return NULL;
    }

    // Initialise les sous-nœuds et les coordonnées du nœud
    node->northwest = node->northeast = node->southwest = node->southeast = NULL;
    node->x = x;
    node->y = y;
    node->size = size;
    node->id = -1; // Initialisation de l'identifiant à -1

    if (image != NULL) {
        // Calcule la couleur moyenne de la région
        node->color = read_image(image, x, y, size);
        // Calcule l'erreur pour cette région
        node->error = calculate_error(image, x, y, size, node->color);
        // Insère le nœud dans le tas max pour le tri par erreur
        insert_max_heap(heap, node);
    }
    return node;
}

void subdivide_quadnode(quadnode *node, MLV_Image *image, max_heap *heap) {
    // Ne subdivise pas si la taille du nœud est inférieure ou égale à 1
    if (node->size <= 1) return;

    // Crée les quatre sous-nœuds et subdivise la région
    node->northwest = create_quadnode(image, node->x, node->y, node->size / 2, heap);
    node->northeast = create_quadnode(image, node->x + node->size / 2, node->y, node->size / 2, heap);
    node->southwest = create_quadnode(image, node->x, node->y + node->size / 2, node->size / 2, heap);
    node->southeast = create_quadnode(image, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, heap);
}

void free_quadnode(quadnode *node) {
    if (node == NULL) return;
    // Libère récursivement la mémoire allouée pour chaque sous-nœud
    free_quadnode(node->northwest);
    free_quadnode(node->northeast);
    free_quadnode(node->southwest);
    free_quadnode(node->southeast);
    // Libère la mémoire du nœud actuel
    free(node);
}

void assign_quadnode_ids(quadnode *node, int *current_id) {
    if (node == NULL || node->id != -1) return;
    // Assigne un identifiant unique au nœud
    node->id = (*current_id)++;
    // Assigne récursivement des identifiants aux sous-nœuds
    assign_quadnode_ids(node->northwest, current_id);
    assign_quadnode_ids(node->northeast, current_id);
    assign_quadnode_ids(node->southwest, current_id);
    assign_quadnode_ids(node->southeast, current_id);
}

void write_minimized_quadnode_to_file(quadnode *node, FILE *file, int isBlackAndWhite) {
    if (node == NULL) return;
    if (node->northwest == NULL) {
        // Le nœud est une feuille
        if (isBlackAndWhite) {
            // Calculer si le nœud est noir ou blanc en utilisant la moyenne des couleurs
            int is_black = (node->color.red + node->color.green + node->color.blue) / 3 < 128 ? 1 : 0;
            // Écrire l'identifiant et la couleur (noir ou blanc) dans le fichier
            fprintf(file, "%d %d\n", node->id, is_black);
        } else {
            // Écrire l'identifiant et les composantes RGBA dans le fichier
            fprintf(file, "%df %d %d %d %d\n", node->id, node->color.red, node->color.green, node->color.blue, node->color.alpha);
        }
    } else {
        // Le nœud est interne, écrire les identifiants des sous-nœuds
        fprintf(file, "%d %d %d %d %d\n", node->id, node->northwest->id, node->northeast->id, node->southwest->id, node->southeast->id);
        // Écriture récursive des sous-nœuds
        write_minimized_quadnode_to_file(node->northwest, file, isBlackAndWhite);
        write_minimized_quadnode_to_file(node->northeast, file, isBlackAndWhite);
        write_minimized_quadnode_to_file(node->southwest, file, isBlackAndWhite);
        write_minimized_quadnode_to_file(node->southeast, file, isBlackAndWhite);
    }
}

void write_quadnode_to_file(quadnode *node, FILE *file, unsigned long long int *buffer, int *bufferSize, int isBlackAndWhite) {
    // Ajoute un bit pour indiquer si le nœud est une feuille ou un nœud interne
    *buffer <<= 1;
    *bufferSize += 1;

    if (node->northwest == NULL) {
        // Le nœud est une feuille
        *buffer += 1;
        if (isBlackAndWhite) {
            // Mode noir et blanc
            *buffer <<= 1;
            *bufferSize += 1;
            // Déterminer si le nœud est noir ou blanc
            int is_black = (node->color.red + node->color.green + node->color.blue) / 3 < 128 ? 1 : 0;
            *buffer += is_black;
        } else {
            // Mode couleur
            *buffer <<= 32;
            *bufferSize += 32;
            // Ajouter les composantes RGBA dans le buffer
            *buffer += ((long long) (node->color.red) % 256) << 24;
            *buffer += ((long long) (node->color.blue) % 256) << 16;
            *buffer += ((long long) (node->color.green) % 256) << 8;
            *buffer += ((long long) (node->color.alpha) % 256);
        }
    }

    // Écrire les bits dans le fichier par octets
    while (*bufferSize >= 8) {
        *bufferSize -= 8;
        char byte = *buffer >> *bufferSize;
        fputc(byte, file);
    }

    // Si le nœud n'est pas une feuille, écrire les sous-nœuds récursivement
    if (node->northwest != NULL) {
        write_quadnode_to_file(node->northwest, file, buffer, bufferSize, isBlackAndWhite);
        write_quadnode_to_file(node->northeast, file, buffer, bufferSize, isBlackAndWhite);
        write_quadnode_to_file(node->southwest, file, buffer, bufferSize, isBlackAndWhite);
        write_quadnode_to_file(node->southeast, file, buffer, bufferSize, isBlackAndWhite);
    }
}

void save_unminimized_quadtree(quadnode *tree, const char *filename, int isBlackAndWhite) {
    // Ouvrir le fichier en mode écriture
    FILE *file = fopen(filename, "w");
    if (!file) {
        // Afficher une erreur si le fichier ne peut pas être ouvert
        perror("Failed to open file for writing");
        return;
    }

    // Initialiser le buffer pour stocker les bits à écrire
    unsigned long long int buffer = 0;
    int bufferSize = 0;

    // Écrire le quadtree dans le fichier
    write_quadnode_to_file(tree, file, &buffer, &bufferSize, isBlackAndWhite);

    // Écrire les bits restants dans le buffer, s'il en reste
    if (bufferSize != 0)
        fputc(buffer << (8 - bufferSize), file);

    // Fermer le fichier
    fclose(file);
}

void save_minimized_quadtree(quadnode *tree, const char *filename, int isBlackAndWhite) {
    // Initialiser l'identifiant courant pour les nœuds
    int current_id = 0;

    // Assigner des identifiants uniques à chaque nœud du quadtree
    assign_quadnode_ids(tree, &current_id);

    // Ouvrir le fichier en mode écriture
    FILE *file = fopen(filename, "w");
    if (!file) {
        // Afficher une erreur si le fichier ne peut pas être ouvert
        perror("Failed to open file for writing");
        return;
    }

    // Écrire le quadtree minimisé dans le fichier
    write_minimized_quadnode_to_file(tree, file, isBlackAndWhite);

    // Fermer le fichier
    fclose(file);
}


void read_quadnode_from_file(quadnode *node, FILE *file, unsigned long long int *buffer, int *bufferSize, int isBlackAndWhite) {
    if (*bufferSize == 0) {
        *buffer = fgetc(file);
        *bufferSize += 8;
    }

    // Vérifie si le nœud est une feuille
    int is_leaf = (*buffer >> (*bufferSize - 1)) % 2;
    *bufferSize -= 1;

    if (is_leaf) {
        // Le nœud est une feuille
        if (isBlackAndWhite) {
            if (*bufferSize == 0) {
                *buffer <<= 8;
                *buffer += fgetc(file);
                *bufferSize += 8;
            }
            // Déterminer si le nœud est noir ou blanc
            int is_black = (*buffer >> (*bufferSize - 1)) % 2;
            pixel current;
            current.red = current.green = current.blue = is_black * 255;
            current.alpha = 255;
            node->color = current;
            *bufferSize -= 1;
        } else {
            // Mode couleur
            for (int i = 0; i < 4; i++) {
                *buffer <<= 8;
                *buffer += fgetc(file);
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
        // Le nœud est un nœud interne, lire les enfants
        node->northwest = create_quadnode(NULL, node->x, node->y, node->size / 2, NULL);
        node->northeast = create_quadnode(NULL, node->x + node->size / 2, node->y, node->size / 2, NULL);
        node->southwest = create_quadnode(NULL, node->x, node->y + node->size / 2, node->size / 2, NULL);
        node->southeast = create_quadnode(NULL, node->x + node->size / 2, node->y + node->size / 2, node->size / 2, NULL);
        // Lire récursivement les sous-nœuds
        read_quadnode_from_file(node->northwest, file, buffer, bufferSize, isBlackAndWhite);
        read_quadnode_from_file(node->northeast, file, buffer, bufferSize, isBlackAndWhite);
        read_quadnode_from_file(node->southwest, file, buffer, bufferSize, isBlackAndWhite);
        read_quadnode_from_file(node->southeast, file, buffer, bufferSize, isBlackAndWhite);
    }
}

quadnode *load_unminimized_quadtree(char *filename, int isBlackAndWhite) {
    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(filename, "r");
    if (!file) {
        // Afficher une erreur si le fichier ne peut pas être ouvert
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Initialiser le buffer pour stocker les bits lus
    unsigned long long int buffer = 0;
    int bufferSize = 0;

    // Créer la racine du quadtree
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, NULL);
    if (!tree) {
        // Fermer le fichier et retourner NULL en cas d'erreur
        fclose(file);
        return NULL;
    }

    // Lire le quadtree à partir du fichier
    read_quadnode_from_file(tree, file, &buffer, &bufferSize, isBlackAndWhite);

    // Fermer le fichier
    fclose(file);

    // Retourner la racine du quadtree
    return tree;
}


int minimise_quadtree(quadnode *tree) {
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

void read_minimized_node_from_file(quadnode *node, FILE *file, int nodeIndex, int isBlackAndWhite) {
    char *line = NULL;
    size_t len;
    int currentIndex;
    char afterNumber;
    pixel currentPixel = {0, 0, 0, 0};

    while (1) {
        int lineValidity = getline(&line, &len, file);
        int scanned = sscanf(line, "%d%c%d %d %d %d", &currentIndex, &afterNumber, 
                             &(currentPixel.red), &(currentPixel.green), 
                             &(currentPixel.blue), &(currentPixel.alpha));

        // Si la ligne est valide et que l'indice actuel est supérieur à nodeIndex, revenir au début du fichier
        if (lineValidity != -1 && currentIndex > nodeIndex)
            rewind(file);

        // Si l'indice actuel correspond à nodeIndex, traiter le nœud
        if (currentIndex == nodeIndex) {
            if ((!isBlackAndWhite && afterNumber == ' ') || (isBlackAndWhite && scanned == 6)) {
                // Crée des sous-nœuds pour le nœud courant
                node->northwest = create_quadnode(NULL, node->x, node->y, node->size/2, NULL);
                node->northeast = create_quadnode(NULL, node->x + node->size/2, node->y, node->size/2, NULL);
                node->southwest = create_quadnode(NULL, node->x, node->y + node->size/2, node->size/2, NULL);
                node->southeast = create_quadnode(NULL, node->x + node->size/2, node->y + node->size/2, node->size/2, NULL);

                // Lire récursivement les sous-nœuds
                read_minimized_node_from_file(node->northwest, file, currentPixel.red, isBlackAndWhite);
                read_minimized_node_from_file(node->northeast, file, currentPixel.green, isBlackAndWhite);
                read_minimized_node_from_file(node->southwest, file, currentPixel.blue, isBlackAndWhite);
                read_minimized_node_from_file(node->southeast, file, currentPixel.alpha, isBlackAndWhite);
            } else if (!isBlackAndWhite && afterNumber == 'f') {
                node->color = currentPixel;  // Assigner la couleur courante au nœud
            } else if (isBlackAndWhite && scanned == 3) {
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

quadnode *load_minimized_quadtree(char *filename, int isBlackAndWhite) {
    // Ouvrir le fichier en mode lecture
    FILE *file = fopen(filename, "r");
    if (!file) {
        // Afficher une erreur si le fichier ne peut pas être ouvert
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Créer la racine du quadtree
    quadnode *tree = create_quadnode(NULL, 0, 0, 512, NULL);
    if (!tree) {
        // Fermer le fichier et retourner NULL en cas d'erreur
        fclose(file);
        return NULL;
    }

    // Lire le quadtree minimisé à partir du fichier
    read_minimized_node_from_file(tree, file, 0, isBlackAndWhite);

    // Fermer le fichier
    fclose(file);

    // Retourner la racine du quadtree
    return tree;
}
