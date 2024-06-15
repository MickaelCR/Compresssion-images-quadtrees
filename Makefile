SRC_DIR = src/
INC_DIR = include/
BIN_DIR = bin/
MODEL_DIR = model/
VIEW_DIR = view/
CONTR_DIR = controller/

CC = gcc
CFLAGS = -Wall -std=c99 -I$(INC_DIR)
LDFLAGS = -lm -lMLV

OBJ = $(BIN_DIR)Main.o $(BIN_DIR)Pixel.o $(BIN_DIR)QuadTree.o $(BIN_DIR)View.o $(BIN_DIR)Controller.o $(BIN_DIR)Heap.o
EXEC = quadtree

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(BIN_DIR)$@ $^ $(LDFLAGS)

$(BIN_DIR)Main.o: $(SRC_DIR)$(CONTR_DIR)Main.c $(INC_DIR)$(CONTR_DIR)Controller.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)Pixel.o: $(SRC_DIR)$(MODEL_DIR)Pixel.c $(INC_DIR)$(MODEL_DIR)Pixel.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)Heap.o: $(SRC_DIR)$(MODEL_DIR)Heap.c $(INC_DIR)$(MODEL_DIR)Heap.h $(INC_DIR)$(MODEL_DIR)QuadTree.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)QuadTree.o: $(SRC_DIR)$(MODEL_DIR)QuadTree.c $(INC_DIR)$(MODEL_DIR)QuadTree.h $(INC_DIR)$(MODEL_DIR)Pixel.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)View.o: $(SRC_DIR)$(VIEW_DIR)View.c $(INC_DIR)$(VIEW_DIR)View.h $(INC_DIR)$(MODEL_DIR)QuadTree.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)Controller.o: $(SRC_DIR)$(CONTR_DIR)Controller.c $(INC_DIR)$(CONTR_DIR)Controller.h $(INC_DIR)$(MODEL_DIR)QuadTree.h $(INC_DIR)$(VIEW_DIR)View.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(BIN_DIR)*.o $(BIN_DIR)$(EXEC)

.PHONY: all clean
