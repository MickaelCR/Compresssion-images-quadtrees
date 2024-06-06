#include "../../include/controller/Controller.h"
#include <stdio.h>
#include <unistd.h>

void process_image(const char *filename) {
	initialize_window();
	MLV_Image *image = MLV_load_image(filename);
	if (!image) {
		fprintf(stderr, "Could not load image %s\n", filename);
		return;
	}

	/*MLV_draw_image(image, 0, 0);
	update_window();*/

	quadnode *tree = create_quadnode(image, 0, 0, 512);
	printf("color=(%d %d %d %d), error=%f\n", tree->color.red, tree->color.green, tree->color.blue, tree->color.alpha, tree->error);
	subdivide(tree, image);
	for (int i=0; i<10000; i++)
		subdivide(find_max_error(tree), image);
	draw_quadtree(tree);

	sleep(3);
	free_window();
	MLV_free_image(image);
}
