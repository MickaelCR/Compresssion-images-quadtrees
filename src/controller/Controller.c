#include "../../include/controller/Controller.h"
#include "../../include/view/View.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <unistd.h>

static void print_quadtree(quadnode *node, int depth) {
    if (node == NULL) {
        return;
    }

    // Indentation pour représenter la profondeur du nœud dans le quadtree
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Afficher les informations du nœud
    printf("Node ID: %d, Color: (%d, %d, %d, %d), Error: %f\n", 
           node->id, node->color.red, node->color.green, node->color.blue, node->color.alpha, node->error);

    // Appeler récursivement la fonction pour les nœuds enfants
    print_quadtree(node->northwest, depth + 1);
    print_quadtree(node->northeast, depth + 1);
    print_quadtree(node->southwest, depth + 1);
    print_quadtree(node->southeast, depth + 1);
}

void process_image(const char *filename) {
    initialize_window();
    MLV_Image *image = MLV_load_image(filename);
    if (!image) {
        fprintf(stderr, "Could not load image %s\n", filename);
        return;
    }

    max_heap *heap = create_max_heap(10);
    quadnode *tree = create_quadnode(image, 0, 0, 512, heap);
    subdivide(tree, image, heap);

    for (int i = 0; i < 100; i++) {
        quadnode *max_error_node = extract_max(heap);
        if (max_error_node && max_error_node->size > 1) {
            subdivide(max_error_node, image, heap);
        }
    }
    int isBW = 0;
    int isMinimized = 1;
    if (isMinimized) {
        minimise_quadtree(tree);
        save_quadtree_minimized(tree, isBW ? "result_minimized.qtn" : "result_minimized.qtc", isBW);
    } else {
        save_quadtree_unminimized(tree, isBW ? "result.qtn" : "result.qtc", isBW);
    }

    heap = create_max_heap(100);
    quadnode *loaded_tree;
    if (isMinimized) {
        loaded_tree = load_quadtree_minimized(isBW ? "result_minimized.qtn" : "result_minimized.qtc", heap, isBW);
    } else {
        loaded_tree = load_quadtree(isBW ? "result.qtn" : "result.qtc", heap, isBW);
    }

    if (loaded_tree) {
        draw_quadtree(loaded_tree);
        free_quadnode(loaded_tree);
    }

    free_max_heap(heap);
    free_quadnode(tree);
    MLV_free_image(image);
    free_window();
}
