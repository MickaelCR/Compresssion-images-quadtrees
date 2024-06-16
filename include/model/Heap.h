/**
 * @file max_heap.h
 * @brief Fichier d'en-tête pour la structure de tas max utilisée dans le projet QuadTree.
 * 
 * Ce fichier contient les définitions et les prototypes de fonctions pour le tas max, qui est utilisé pour gérer les nœuds du QuadTree en fonction de leurs valeurs d'erreur.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#ifndef MAX_HEAP_H
#define MAX_HEAP_H

/**
 * @brief Déclaration anticipée de la structure quadnode.
 */
struct _quadnode_;
typedef struct _quadnode_ quadnode;

/**
 * @brief Structure représentant un tas max.
 */
typedef struct {
    quadnode **nodes; /**< Tableau de pointeurs vers les quadnodes. */
    int size; /**< Nombre actuel d'éléments dans le tas. */
    int capacity; /**< Capacité maximale du tas. */
} max_heap;

/**
 * @brief Crée un tas max avec une capacité donnée.
 * 
 * @param capacity La capacité maximale du tas.
 * @return Un pointeur vers le tas max créé.
 */
max_heap* create_max_heap(int capacity);

/**
 * @brief Insère un nœud dans le tas max.
 * 
 * @param heap Un pointeur vers le tas max.
 * @param node Un pointeur vers le nœud à insérer.
 */
void insert_max_heap(max_heap *heap, quadnode *node);

/**
 * @brief Extrait le nœud avec la valeur maximale du tas max.
 * 
 * @param heap Un pointeur vers le tas max.
 * @return Un pointeur vers le nœud extrait.
 */
quadnode* extract_max(max_heap *heap);

/**
 * @brief Libère la mémoire allouée pour le tas max.
 * 
 * @param heap Un pointeur vers le tas max à libérer.
 */
void free_max_heap(max_heap *heap);

#endif // MAX_HEAP_H