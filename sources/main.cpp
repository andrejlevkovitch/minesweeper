//main.cpp - the basis file

#include<iostream>
#include<curses.h>
#include"../include/minesweeperhead.hpp"

int main (void)
{
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    init_colors();

    do{
        clear();
        Minefield game{Complexity{set_complexity()}};
        game.game();
        printw("Do you want play again? (y/n)\n");
    } while(getch() == 'y');

    endwin();

    return 0;
}
