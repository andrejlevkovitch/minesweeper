//complexity.cpp

#include"../../include/minesweeperhead.hpp"
#include<iostream>
#include<curses.h>
#include<unistd.h>

Complexity::Complexity(int cx)
{
    switch(cx) {
        case EASILY:
            sizes_ = Koords{MINY, MINX};
            mines_ = 10;
            break;
        case MEDIUM:
            sizes_ = Koords{16, 16};
            mines_ = 40;
            break;
        case COMPLICATED:
            sizes_ = Koords{16, 30};
            mines_ = 99;
            break;
        case EXPERT:
        default:
            move(0, 0);
            clear();
            echo();
            int sizeY{}, sizeX{};
            printw("Input size Y(%i <= Y <= %i):\n", MINY, MAXY);
            refresh();
            scanw("%hu", &sizeY);
            printw("Input size X(%i <= Y <= %i):\n", MINX, MAXX);
            refresh();
            scanw("%hu", &sizeX);
            if (sizeY > MAXY)
                sizeY = MAXY;
            else if (sizeY < MINY)
                sizeY = MINY;
            if (sizeX > MAXX)
                sizeX = MAXX;
            else if (sizeX < MINX)
                sizeX = MINX;
            sizes_ = Koords{sizeY, sizeX};
            printw("You input: y = %hu, x = %hu.\n", sizeY, sizeX);
            refresh();

            int totalSize = sizeY * sizeX;
            printw("Input number of mines(%i <= mines < %i):\n", MINMINES, totalSize);
            refresh();
            scanw("%hu", &mines_);
            if (mines_ < MINMINES || mines_ >= totalSize) {
                mines_ = totalSize / (4 + rand() % 5);
            }
            printw("You input: mines = %hu.\n", mines_);
            refresh();
            noecho();
            sleep(1);
            clear();
            break;
    }
}

auto Complexity::getM() const -> decltype(mines_)
{
    return mines_;
}

auto Complexity::getY() const -> decltype(sizes_.getY())
{
    return sizes_.getY();
}

auto Complexity::getX() const -> decltype(sizes_.getX())
{
    return sizes_.getX();
}
