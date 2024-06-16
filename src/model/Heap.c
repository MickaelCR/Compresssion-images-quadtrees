/**
 * @file Heap.c
 * @brief Fichier source pour la gestion d'un tas (max heap) utilisé dans les opérations sur les quadtree.
 * 
 * Ce fichier contient les définitions des fonctions pour créer, insérer, extraire et libérer un tas max 
 * utilisé pour optimiser les opérations de subdivision dans les quadtree.
 * 
 * @auteurs 
 * Mickaël Rakotoarison 
 * Romain Buges
 * @date 2024
 */

#include "../../include/model/Heap.h"
#include "../../include/model/QuadTree.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Échange deux pointeurs de nœuds dans le tas.
 * 
 * @param a Pointeur vers le premier nœud.
 * @param b Pointeur vers le second nœud.
 */
static void swap(quadnode **a, quadnode **b) {
    // Échange les deux pointeurs de nœuds
    quadnode *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Crée un tas max avec une capacité donnée.
 * 
 * @param capacity La capacité maximale du tas.
 * @return Un pointeur vers le tas max créé.
 */
max_heap* create_max_heap(int capacity) {
    // Alloue de la mémoire pour le tas
    max_heap *heap = malloc(sizeof(max_heap));
    heap->size = 0;
    heap->capacity = capacity;
    // Alloue de la mémoire pour les nœuds du tas
    heap->nodes = malloc(capacity * sizeof(quadnode *));
    return heap;
}

/**
 * @brief Insère un nœud dans le tas max.
 * 
 * @param heap Un pointeur vers le tas max.
 * @param node Un pointeur vers le nœud à insérer.
 */
void insert_max_heap(max_heap *heap, quadnode *node) {
    // Double la capacité du tas si nécessaire
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(quadnode *));
    }

    // Ajoute le nouveau nœud à la fin du tas
    heap->nodes[heap->size] = node;
    int current = heap->size;
    heap->size++;

    // Remonte le nœud tant qu'il est plus grand que son parent
    while (current > 0 && heap->nodes[current]->error > heap->nodes[(current - 1) / 2]->error) {
        swap(&heap->nodes[current], &heap->nodes[(current - 1) / 2]);
        current = (current - 1) / 2;
    }
}

/**
 * @brief Extrait le nœud avec la valeur maximale du tas max.
 * 
 * @param heap Un pointeur vers le tas max.
 * @return Un pointeur vers le nœud extrait.
 */
quadnode* extract_max(max_heap *heap) {
    if (heap->size == 0) {
        return NULL;
    }

    // Récupère le nœud max (racine du tas)
    quadnode *max = heap->nodes[0];
    // Place le dernier nœud à la racine et réduit la taille du tas
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    int current = 0;
    // Descend le nœud pour restaurer la propriété du tas max
    while (current < heap->size) {
        int left = 2 * current + 1;
        int right = 2 * current + 2;
        int largest = current;

        // Trouve le plus grand des deux enfants
        if (left < heap->size && heap->nodes[left]->error > heap->nodes[largest]->error) {
            largest = left;
        }
        if (right < heap->size && heap->nodes[right]->error > heap->nodes[largest]->error) {
            largest = right;
        }
        // Si le plus grand enfant est plus grand que le nœud actuel, échange-les
        if (largest != current) {
            swap(&heap->nodes[current], &heap->nodes[largest]);
            current = largest;
        } else {
            break;
        }
    }

    return max;
}

/**
 * @brief Libère la mémoire allouée pour le tas max.
 * 
 * @param heap Un pointeur vers le tas max à libérer.
 */
void free_max_heap(max_heap *heap) {
    free(heap->nodes);
    free(heap);
}
