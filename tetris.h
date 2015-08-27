#ifndef TETRIS_H

#define TETRIS_H

#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define ROWS 30
#define COLS 15
#define MAX 4
#define MIN 0

struct Cell {
    int y;
    int x;
    int color;
};
struct Vector {
    int y;
    int x;
};
struct Square {
    struct Cell cells[4];
    struct Vector vector[4][4];
};

extern WINDOW * game_win;

void init();
void draw();
void drop();
void clean();
void start();
void change();
void newSquare();
void step(int dir);
bool check(int dir);

#endif