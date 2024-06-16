#include "../../include/model/Heap.h"
#include "../../include/model/QuadTree.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(quadnode **a, quadnode **b) {
    quadnode *temp = *a;
    *a = *b;
    *b = temp;
}

max_heap* create_max_heap(int capacity) {
    max_heap *heap = malloc(sizeof(max_heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->nodes = malloc(capacity * sizeof(quadnode *));
    return heap;
}

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

void free_max_heap(max_heap *heap) {
    free(heap->nodes);
    free(heap);
}
