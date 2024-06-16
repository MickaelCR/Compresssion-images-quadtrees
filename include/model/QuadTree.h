/**
 * @file QuadTree.h
 * @brief Fichier d'en-tête pour les opérations sur les quadtree utilisés dans le projet.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour les opérations sur les quadtree, telles que la création, la subdivision, la minimisation, la sauvegarde et le chargement des quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#ifndef MODEL_QUADTREE_H
#define MODEL_QUADTREE_H

#include <MLV/MLV_all.h>
#include "../model/Pixel.h"

/**
 * @brief Structure représentant un nœud de quadtree.
 */
typedef struct _quadnode_ {
    pixel color; /**< Couleur du nœud. */
    struct _quadnode_ *northwest; /**< Pointeur vers le nœud nord-ouest. */
    struct _quadnode_ *northeast; /**< Pointeur vers le nœud nord-est. */
    struct _quadnode_ *southwest; /**< Pointeur vers le nœud sud-ouest. */
    struct _quadnode_ *southeast; /**< Pointeur vers le nœud sud-est. */
    int x; /**< Coordonnée x du nœud. */
    int y; /**< Coordonnée y du nœud. */
    int size; /**< Taille du nœud. */
    double error; /**< Erreur de compression du nœud. */
    int id; /**< Identifiant unique du nœud. */
} quadnode;

#include "../model/Heap.h"

/**
 * @brief Crée un nœud de quadtree sans image.
 * 
 * @param id Identifiant du nœud.
 * @return Pointeur vers le nœud créé.
 */
quadnode *create_quadnode_no_image(int id);

/**
 * @brief Trouve le nœud avec l'erreur maximale dans un quadtree.
 * 
 * @param node Pointeur vers le nœud racine.
 * @return Pointeur vers le nœud avec l'erreur maximale.
 */
quadnode *find_max_error(quadnode *node);

/**
 * @brief Crée un nœud de quadtree à partir d'une image.
 * 
 * @param image L'image à partir de laquelle créer le nœud.
 * @param x Coordonnée x du nœud.
 * @param y Coordonnée y du nœud.
 * @param size Taille du nœud.
 * @param heap Tas max pour gérer les erreurs de compression.
 * @return Pointeur vers le nœud créé.
 */
quadnode *create_quadnode(MLV_Image *image, int x, int y, int size, max_heap *heap);

/**
 * @brief Subdivise un nœud de quadtree en quatre sous-nœuds.
 * 
 * @param node Pointeur vers le nœud à subdiviser.
 * @param image L'image à partir de laquelle créer les sous-nœuds.
 * @param heap Tas max pour gérer les erreurs de compression.
 */
void subdivide_quadnode(quadnode *node, MLV_Image *image, max_heap *heap);

/**
 * @brief Libère la mémoire allouée pour un nœud de quadtree et ses sous-nœuds.
 * 
 * @param node Pointeur vers le nœud à libérer.
 */
void free_quadnode(quadnode *node);

/**
 * @brief Charge un quadtree non minimisé à partir d'un fichier.
 * 
 * @param filename Nom du fichier à partir duquel charger le quadtree.
 * @param isBlackAndWhite Indique si l'image est en noir et blanc.
 * @return Pointeur vers le quadtree chargé.
 */
quadnode *load_unminimized_quadtree(char *filename, int isBlackAndWhite);

/**
 * @brief Minimise un quadtree.
 * 
 * @param tree Pointeur vers le quadtree à minimiser.
 * @return 1 si le quadtree a été minimisé, 0 sinon.
 */
int minimise_quadtree(quadnode *tree);

/**
 * @brief Charge un quadtree minimisé à partir d'un fichier.
 * 
 * @param filename Nom du fichier à partir duquel charger le quadtree.
 * @param blackAndWhite Indique si l'image est en noir et blanc.
 * @return Pointeur vers le quadtree chargé.
 */
quadnode *load_minimized_quadtree(char *filename, int blackAndWhite);

/**
 * @brief Assigne des identifiants uniques à chaque nœud d'un quadtree.
 * 
 * @param node Pointeur vers le nœud racine.
 * @param current_id Pointeur vers l'identifiant courant.
 */
void assign_ids(quadnode *node, int *current_id);

/**
 * @brief Écrit un nœud de quadtree minimisé dans un fichier.
 * 
 * @param node Pointeur vers le nœud à écrire.
 * @param fptr Pointeur vers le fichier.
 * @param blackAndWhite Indique si l'image est en noir et blanc.
 */
void write_minimized_node_to_file(quadnode *node, FILE *fptr, int blackAndWhite);

/**
 * @brief Sauvegarde un quadtree non minimisé dans un fichier.
 * 
 * @param tree Pointeur vers le quadtree à sauvegarder.
 * @param filename Nom du fichier.
 * @param isBlackAndWhite Indique si l'image est en noir et blanc.
 */
void save_unminimized_quadtree(quadnode *tree, const char *filename, int isBlackAndWhite);

/**
 * @brief Sauvegarde un quadtree minimisé dans un fichier.
 * 
 * @param tree Pointeur vers le quadtree à sauvegarder.
 * @param filename Nom du fichier.
 * @param isBlackAndWhite Indique si l'image est en noir et blanc.
 */
void save_minimized_quadtree(quadnode *tree, const char *filename, int isBlackAndWhite);

#endif // MODEL_QUADTREE_H
