SRC = main.c tetris.c
OBJ = main.o tetris.o
PROG = tetris

$(PROG): $(OBJ)
	gcc -std=c11 -lncurses $(OBJ) -o $(PROG)

$(OBJ): $(SRC)
