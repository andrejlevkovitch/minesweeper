//set_complexity.cpp

#include"../../include/minesweeperhead.hpp"
#include<curses.h>
#include<stdlib.h>

int set_complexity(void)
{
    for (int i{}; i < elements_of_complexity; ++i) {
        printw("%s\n", OPTIONS[i]);
    }
    int y{}, x{};
    move(y, x);
    refresh();

    int ch{};
    for (;;) {
        switch (ch = getch()) {
            case KEY_DOWN:
                if (y < elements_of_complexity - 1)
                    move(++y, x);
                    refresh();
                break;
            case KEY_UP:
                if (y > 0)
                    move(--y, x);
                    refresh();
                break;
            case ENTER:
                clear();
                return y;
                break;
            case ESC: case '':
                endwin();
                exit(EXIT_FAILURE);
                break;
            default:
                break;
        }
    }
}
