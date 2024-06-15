#include "../../include/controller/Controller.h"
#include "../../include/view/View.h"
#include "../../include/model/Heap.h"
#include <stdio.h>
#include <unistd.h>

void process_image(MLV_Image *image) {
    // Créer le tas max
    max_heap *heap = create_max_heap(1000);

    // Créer l'arbre quadtree
    quadnode *tree = create_quadnode(image, 0, 0, 512, heap);
    printf("Initial color=(%d %d %d %d), error=%f\n", tree->color.red, tree->color.green, tree->color.blue, tree->color.alpha, tree->error);

    // Subdiviser les nœuds en utilisant le tas max pour trouver l'erreur maximale
    for (int i = 0; i < 10000; i++) {
        quadnode *max_error_node = extract_max(heap);
        if (max_error_node && max_error_node->error > 100000) { // Assurez-vous de ne pas subdiviser les nœuds de taille 1x1
            subdivide(max_error_node, image, heap);
            draw_quadtree(tree); // Redessiner le quadtree après chaque subdivision
        }
    }

    // Sauvegarder le quadtree
    save_quadtree(tree, "result.qtc");

    // Charger le quadtree depuis le fichier
    tree = load_quadtree("result.qtc");
    minimise_quadtree(tree);
    draw_quadtree(tree);
    sleep(3);

    tree = load_minimised_quadtree("test", 1);
    draw_quadtree(tree);
    sleep(3);    

    // Libérer les ressources
    free_max_heap(heap);
    free_quadnode(tree);
}
