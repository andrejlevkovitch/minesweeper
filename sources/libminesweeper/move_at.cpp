//move_at.cpp

#include"../../include/minesweeperhead.hpp"
#include<curses.h>

void move_at(const Koords &in)
{
    move(in.getY(), in.getX());
    return;
}
