#include "../../include/controller/Controller.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    process_image(argv[1]);
    return 0;
}
