#include <ncurses.h>
#include "tetris.h"

int main() {
    initscr();
    noecho();
    keypad(stdscr , TRUE);
    init();
    delwin(game_win);
    endwin();
}
