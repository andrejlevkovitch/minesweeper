//init_colors.cpp

#include"../../include/minesweeperhead.hpp"
#include<curses.h>
#include<iostream>

int init_colors(void)
{
    if (!has_colors())
    {
        std::cout << "Colors are not supported!\n";
        return FALSE;
    }
    start_color();

    init_pair(FREE_CELL, COLOR_BLACK, COLOR_WHITE);
    init_pair(ONE_CELL, COLOR_BLUE, COLOR_WHITE);
    init_pair(TWO_CELL, COLOR_GREEN, COLOR_WHITE);
    init_pair(MORE_CELL, COLOR_RED, COLOR_WHITE);
    init_pair(MINE_CELL, COLOR_BLACK, COLOR_RED);
    init_pair(FRAME_CELL, COLOR_BLACK, COLOR_WHITE);
    init_pair(VOID_CELL, COLOR_BLACK, COLOR_BLUE);

    return TRUE;
}
