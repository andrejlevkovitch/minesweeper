//minesweeperhead.hpp - haderfile for minesweeper-project

#ifndef MW_HPP
#define MW_HPP

#include<iostream>
#include<vector>
#include<curses.h>

const int sleepingTime{2};

const int BYSCREEN{5};
const int BXSCREEN{9};

const int ESC{033};
const int ENTER{012};
const int FLAG{'f'};

//all cells
enum {DEFAULT_CELL, FREE_CELL, ONE_CELL, TWO_CELL, MORE_CELL, MINE_CELL, FRAME_CELL, VOID_CELL};

const chtype MINE{'M' | COLOR_PAIR(MINE_CELL)};
const chtype DEF_VALUE{' ' | COLOR_PAIR(FREE_CELL)};
const chtype OPEN_VALUE{'O' | COLOR_PAIR(FREE_CELL)};
const chtype VOID_VALUE{' ' | COLOR_PAIR(VOID_CELL)};
const chtype FLAG_VALUE{'?' | COLOR_PAIR(MINE_CELL)};

const int MINY{9};
const int MINX{9};
const int MAXY{45};
const int MAXX{90};
const int MINMINES{10};
const int Y{0};
const int X{1};

//complexity
enum {EASILY, MEDIUM, COMPLICATED, EXPERT};
const int elements_of_complexity{4};
const char *const OPTIONS[elements_of_complexity] {"EASILY", "MEDIUM", "COMPLICATED", "EXPERT"};

class Complexity;

class Koords {
    private:
        unsigned short y_;
        unsigned short x_;
    public:
        Koords(const int y = 0, const int x = 0);
        auto getY() const -> decltype(y_);
        auto getX() const -> decltype(x_);
        friend Complexity;
};

class Complexity {
    private:
        Koords sizes_;
        unsigned short mines_;
    public:
        explicit Complexity(int = EASILY);
        auto getY() const -> decltype(sizes_.getY());
        auto getX() const -> decltype(sizes_.getX());
        auto getM() const -> decltype(mines_);
};

class Minefield {
    private:
        std::vector<std::vector<chtype>> field_;
        mutable std::vector<std::vector<chtype>> screen_;
        Koords beginen_;
        Koords end_;
        unsigned short minesN_;
    public:
        Minefield(Complexity = Complexity{EASILY});
        ~Minefield();
        void print() const;
        void show() const;
        void game(); 
    private:
        void field_generate(const Koords &first_cell);
        void frame() const;
        chtype open_cell(const Koords &) const;
        void print_screen() const;
        void set_flag(const Koords &) const;
        void print_value(const int i, const int j) const;
        Koords koords_to_relative(const Koords &) const;
};

int init_colors(void);
int set_complexity(void);
void move_at(const Koords &);

#endif
