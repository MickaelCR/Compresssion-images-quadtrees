#include "../../include/controller/Controller.h"
#include "../../include/view/View.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <unistd.h>

void process_image(const char *filename) {
    initialize_window();
    MLV_Image *image = MLV_load_image(filename);
    if (!image) {
        fprintf(stderr, "Could not load image %s\n", filename);
        return;
    }

    // Créer le tas max
    max_heap *heap = create_max_heap(10);

    // Créer l'arbre quadtree
    quadnode *tree = create_quadnode(image, 0, 0, 512, heap);
    int isBW = 1;
    printf("Initial color=(%d %d %d %d), error=%f\n", tree->color.red, tree->color.green, tree->color.blue, tree->color.alpha, tree->error);

    // Subdiviser initialement le nœud racine
    subdivide(tree, image, heap);

    // Subdiviser les nœuds en utilisant le tas max pour trouver l'erreur maximale
    for (int i = 0; i < 10000; i++) {
        quadnode *max_error_node = extract_max(heap);
        if (max_error_node && max_error_node->size > 1) {
            subdivide(max_error_node, image, heap);
        }
    }

    // Sauvegarder le quadtree en fonction de isBW
    if (isBW) {
        save_quadtree_bw(tree, "result_bw.qtc");
    } else {
        save_quadtree(tree, "result.qtc");
    }

    heap = create_max_heap(100);

    // Charger le quadtree depuis le fichier en fonction de isBW
    quadnode *loaded_tree;
    if (isBW) {
        loaded_tree = load_quadtree_bw("result_bw.qtc", heap);
    } else {
        loaded_tree = load_quadtree("result.qtc", heap);
    }

    if (loaded_tree) {
        // Afficher le quadtree chargé
        draw_quadtree(loaded_tree);
        free(loaded_tree);
    }

    // Libérer les ressources
    free_max_heap(heap);
    free(tree);
    MLV_free_image(image);
    free_window();
}
