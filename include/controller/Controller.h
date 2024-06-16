#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <MLV/MLV_all.h>

#include "../model/QuadTree.h"
#include "../model/Heap.h"
#include "../view/View.h"

quadnode *process_image(MLV_Image* image, int useCircles);

#endif
