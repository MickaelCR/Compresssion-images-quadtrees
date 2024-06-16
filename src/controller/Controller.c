#include "../../include/controller/Controller.h"
#include "../../include/view/View.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <unistd.h>

quadnode *process_image(MLV_Image *image, int useCircles) {
    // Créer le tas max
    max_heap *heap = create_max_heap(1000);

    // Créer l'arbre quadtree
    quadnode *tree = create_quadnode(image, 0, 0, 512, heap);
    int multiple = 1;

    // Boucle pour subdiviser l'image en fonction de l'erreur maximale
    for (int i = 0; i < 10000; i++) {
        quadnode *max_error_node = extract_max(heap);
        if (max_error_node && max_error_node->size > 1) {
            subdivide_quadnode(max_error_node, image, heap);
        }

        // Affiche l'arbre quadtree à des intervalles exponentiels
        if (multiple == i) {
            multiple *= 2;
            MLV_draw_filled_rectangle(0, 0, 512, 512, MLV_COLOR_WHITE);
            draw_quadtree(tree, useCircles);
            update_window();
            sleep(1);
        }
    }

    // Libère le tas max
    free_max_heap(heap);
    return tree;
}
