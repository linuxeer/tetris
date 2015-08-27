#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "tetris.h"

int veci = 0;
struct Square curr;
int game_board[ROWS][COLS];
struct Square all_squares[5];

struct Cell cells_a[] = {1 , 6 , 1 , 1 , 7 , 2 , 1 , 8 , 3 , 2 , 7 , 4};
struct Cell cells_b[] = {0 , 7 , 1 , 0 , 8 , 2 , 1 , 7 , 3 , 1 , 8 , 4};
struct Cell cells_c[] = {0 , 7 , 1 , 0 , 8 , 2 , 0 , 9 , 3 , 0 , 10 , 4};
struct Cell cells_d[] = {0 , 7 , 1 , 0 , 8 , 2 , 0 , 9 , 3 , 1 , 9 , 4};
struct Cell cells_e[] = {0 , 7 , 1 , 0 , 8 , 2 , 1 , 6 , 3 , 1 , 7 , 4};

struct Vector vector_a[4][4] = {1 , 1 , 0 , 0 , -1 , -1 , -1 , 1 , -1 , 1 , 0 , 0 , 1 , -1 , -1 , -1 , -1 , -1 , 0 , 0 , 1 , 1 , 1 , -1 , 1 , -1 , 0 , 0 , -1 , 1 , 1 , 1};
struct Vector vector_b[4][4];
struct Vector vector_c[4][4] = {-1 , 1 , 0 , 0 , 1 , -1 , 2 , -2 , 1 , 1 , 0 , 0 , -1 , -1 , -2 , -2 , 1 , -1 , 0 , 0 , -1 , 1 , -2 , 2 , -1 , -1 , 0 , 0 , 1 , 1 , 2 , 2};
struct Vector vector_d[4][4] = {-2 , 2 , -1 , 1 , 0 , 0 , -1 , -1 , 2 , 2 , 1 , 1 , 0 , 0 , -1 , 1 , 2 , -2 , 1 , -1 , 0 , 0 , 1 , 1 , -2 , -2 , -1 , -1 , 0 , 0 , 1 , -1};
struct Vector vector_e[4][4] = {1 , 1 , 2 , 0 , -1 , 1 , 0 , 0 , 1 , -1 , 0 , -2 , 1 , 1 , 0 , 0 , -1 , -1 , -2 , 0 , 1 , -1 , 0 , 0 , -1 , 1 , 0 , 2 , -1 , -1 , 0 , 0};

WINDOW * game_win;

void cslog() {
    int start_y = 5;
    int start_x = 50;
    for (int i = 0 ; i < ROWS ; i++) {
        for (int j = 0 ; j < COLS ; j++) {
            mvprintw(start_y + i , start_x + j * 2 , "%i " , game_board[i][j]);
        }
        printw("\n");
    }
}

void draw() {
    cslog();
    for (int i = 0 ; i < ROWS ; i++) {
        for (int j = 0 ; j < COLS ; j++) {
            mvwprintw(game_win , i , j * 2 , "  ");
            if (game_board[i][j]) {
                wattron(game_win , COLOR_PAIR(game_board[i][j]));
                mvwprintw(game_win , i , j * 2 , "  ");
                wattroff(game_win , COLOR_PAIR(game_board[i][j]));
            }
        }
    }
    box(game_win , 0 , 0);
    wrefresh(game_win);
}

void step(int dir) {
    bool movable = check(dir);
    if (!movable) {
        if (dir == DOWN) {
            clean();
            newSquare();
        }
        return;
    }
    for (int i = 0 ; i < 4 ; i++) {
        game_board[curr.cells[i].y][curr.cells[i].x] = 0;
    }
    for (int i = 0 ; i < 4 ; i++) {
        switch (dir) {
            case DOWN:
                curr.cells[i].y++;
                break;
            case LEFT:
                curr.cells[i].x--;
                break;
            case RIGHT:
                curr.cells[i].x++;
                break;
        }
        game_board[curr.cells[i].y][curr.cells[i].x] = curr.cells[i].color;
    }
    draw();
}

bool check(int dir) {
    bool res = true;
    for (int i = 0 ; i < 4 ; i++) {
        game_board[curr.cells[i].y][curr.cells[i].x] = 0;
    }
    for (int i = 0 ; i < 4 ; i++) {
        int y = curr.cells[i].y;
        int x = curr.cells[i].x;
        switch (dir) {
            case DOWN:
                if (y == 28 || game_board[y + 1][x]) {
                    res = false;
                }
                break;
            case LEFT:
                if (x == 1 || game_board[y][x - 1]) {
                    res = false;
                }
                break;
            case RIGHT:
                if (x == 13 || game_board[y][x + 1]) {
                    res = false;
                }
                break;
        }
    }
    for (int i = 0 ; i < 4 ; i++) {
        game_board[curr.cells[i].y][curr.cells[i].x] = curr.cells[i].color;
    }
    return res;
}

void start() {
    bool playing = true;
    while (playing) {
        int ch = getch();
        switch (ch) {
            case KEY_LEFT:
                step(LEFT);
                break;
            case KEY_RIGHT:
                step(RIGHT);
                break;
            case KEY_DOWN:
                step(DOWN);
                break;
            case 'q':
                playing = false;
                break;
            case ' ':
                change();
                break;
        }
    }
}

void drop() {
    step(DOWN);
    signal(SIGALRM , drop);
    alarm(1);
}

void newSquare() {
    int index = rand() % (MAX + 1 - MIN) + MIN;
    curr = all_squares[index];
    veci = 0;
}

void change() {
    struct Vector * vec = curr.vector[veci];
    for (int i = 0 ; i < 4 ; i++) {
        game_board[curr.cells[i].y][curr.cells[i].x] = 0;
    }
    for (int i = 0 ; i < 4 ; i++) {
        curr.cells[i].x += vec[i].x;
        curr.cells[i].y += vec[i].y;
        game_board[curr.cells[i].y][curr.cells[i].x] = curr.cells[i].color;
    }
    draw();
    if (++veci == 4) {
        veci = 0;
    }
}

void clean() {
    for (int i = ROWS - 1 ; i > 0 ; i--) {
        for (int j = 1 ; j < COLS - 1 ; j++) {
            if (!game_board[i][j]) {
                break;
            }
            if (j == COLS - 2) {
                for (int k = i ; k > 0 ; k--) {
                    for (int z = 0 ; z < COLS ; z++) {
                        game_board[k][z] = game_board[k - 1][z];
                    }
                }
                i++;
            }
        }
    }
    draw();
}

void init() {
    start_color();
    init_pair(1 , COLOR_RED , COLOR_YELLOW);
    init_pair(2 , COLOR_RED , COLOR_RED);
    init_pair(3 , COLOR_RED , COLOR_BLUE);
    init_pair(4 , COLOR_RED , COLOR_CYAN);

    memcpy(&all_squares[0].cells[0] , &cells_a[0] , sizeof(struct Cell) * 4);
    memcpy(&all_squares[1].cells[0] , &cells_b[0] , sizeof(struct Cell) * 4);
    memcpy(&all_squares[2].cells[0] , &cells_c[0] , sizeof(struct Cell) * 4);
    memcpy(&all_squares[3].cells[0] , &cells_d[0] , sizeof(struct Cell) * 4);
    memcpy(&all_squares[4].cells[0] , &cells_e[0] , sizeof(struct Cell) * 4);

    memcpy(&all_squares[0].vector[0][0] , &vector_a[0][0] , sizeof(struct Vector) * 16);
    memcpy(&all_squares[1].vector[0][0] , &vector_b[0][0] , sizeof(struct Vector) * 16);
    memcpy(&all_squares[2].vector[0][0] , &vector_c[0][0] , sizeof(struct Vector) * 16);
    memcpy(&all_squares[3].vector[0][0] , &vector_d[0][0] , sizeof(struct Vector) * 16);
    memcpy(&all_squares[4].vector[0][0] , &vector_e[0][0] , sizeof(struct Vector) * 16);

    game_win = newwin(30 , 30 , 5 , 10);
    newSquare();
    curs_set(0);
    printw("welcome to play! press q to exit game!");
    drop();
    start();
}
