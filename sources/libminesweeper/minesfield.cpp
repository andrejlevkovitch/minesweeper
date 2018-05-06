//minesfield.cpp

#include"../../include/minesweeperhead.hpp"
#include<vector>
#include<cstdlib>
#include<ctime>
#include<curses.h>
#include<unistd.h>
#include<tuple>

Minefield::Minefield(Complexity cx) : beginen_(BYSCREEN, BXSCREEN), minesN_(cx.getM())
{
    srand(time(NULL));

    screen_.resize(cx.getY(), std::remove_reference_t<decltype(screen_[0])> (cx.getX(), VOID_VALUE));

    {
        int ey{}, ex{};
        ey = beginen_.getY() + screen_.size() + 1;
        ex = beginen_.getX() + screen_[0].size() + screen_[0].size() + 1;
        end_ = Koords{ey, ex};
    }
}

Minefield::~Minefield()
{
}

Koords Minefield::koords_to_relative(const Koords &in) const
{
    return Koords{in.getY() - (beginen_.getY() + 1), (in.getX() - (beginen_.getX() + 1)) / 2};
}

void Minefield::field_generate(const Koords &first_cell)
{
    Koords opened_cell = koords_to_relative(first_cell);
    field_ = screen_;
    for (int i{}; i < field_.size(); ++i) {
        for (int j{}; j < field_[i].size(); ++j) {
            field_[i][j] = DEF_VALUE;
        }
    }

    std::vector<Koords> allCells;
    std::remove_reference_t<decltype(allCells)>::iterator koordMine{};
    std::vector<Koords> mineStore(minesN_);
    allCells.reserve(field_.size() * field_[0]. size());
    for (int i{}; i < field_.size(); ++i) {
        for (int j{}; j < field_[i].size(); ++j) {
            if (i == opened_cell.getY() && j == opened_cell.getX()) {
            }
            else
                allCells.push_back(Koords{i, j});
        }
    }
    for (int i{}; i < mineStore.size(); ++i) {
        koordMine = allCells.begin() + rand() % allCells.size();
        field_[(*koordMine).getY()][(*koordMine).getX()] = MINE;
        mineStore[i] = *koordMine;
        allCells.erase(koordMine);
    }

    for (int i{}; i < mineStore.size(); ++i) {
        for (int y{mineStore[i].getY() - 1}; y < mineStore[i].getY() + 2; ++y) {
            if (y >= 0 && y < field_.size()) {
                for (int x{mineStore[i].getX() - 1}; x < mineStore[i].getX() + 2; ++x) {
                    if (x >= 0 && x < field_[y].size() && field_[y][x] != MINE) {
                        if (field_[y][x] == DEF_VALUE)
                            field_[y][x] = '1' | COLOR_PAIR(ONE_CELL);
                        else {
                            if ((field_[y][x] & ~A_ATTRIBUTES) == '1')
                                field_[y][x] = (++field_[y][x] & ~A_ATTRIBUTES) | COLOR_PAIR(TWO_CELL);
                            else
                                field_[y][x] = (++field_[y][x] & ~A_ATTRIBUTES) | COLOR_PAIR(MORE_CELL);
                        }
                    }
                }
            }
        }
    }
    return;
}

void Minefield::frame() const
{
    int y{beginen_.getY()}, x{beginen_.getX()};
    attron(COLOR_PAIR(FRAME_CELL));
    mvaddch(y, x, ACS_ULCORNER);
    for (int i{}; i < screen_[0].size(); ++i) {
        addch(ACS_HLINE);
        addch(ACS_HLINE);
    }
    addch(ACS_URCORNER);
    y++;
    for (int i{}; i < screen_.size(); ++i, y = ++y) {
        mvaddch(y, x, ACS_VLINE);
        mvaddch(y, end_.getX(), ACS_VLINE);
    }
    mvaddch(y, x, ACS_LLCORNER);
    for (int i{}; i < screen_[0].size(); ++i) {
        addch(ACS_HLINE);
        addch(ACS_HLINE);
    }
    addch(ACS_LRCORNER);
    attroff(COLOR_PAIR(FRAME_CELL));

    print_screen();
    return;
}

void Minefield::game()
{
    frame();
    int flagsN{};
    Koords currentKoords{beginen_.getY() + 1, beginen_.getX() + 1};
    move_at(currentKoords);
    refresh();
    int ch{};
    do {
        switch (ch = getch()) {
            case KEY_UP:
                if (currentKoords.getY() > beginen_.getY() + 1) {
                    currentKoords = Koords{currentKoords.getY() - 1, currentKoords.getX()};
                    move_at(currentKoords);
                    refresh();
                }
                break;
            case KEY_DOWN:
                if (currentKoords.getY() < end_.getY() - 1) {
                    currentKoords = Koords{currentKoords.getY() + 1, currentKoords.getX()};
                    move_at(currentKoords);
                    refresh();
                }
                break;
            case KEY_LEFT:
                if (currentKoords.getX() > beginen_.getX() + 1) {
                    currentKoords = Koords{currentKoords.getY(), currentKoords.getX() - 2};
                    move_at(currentKoords);
                    refresh();
                }
                break;
            case KEY_RIGHT:
                if (currentKoords.getX() < end_.getX() - 2) {
                    currentKoords = Koords{currentKoords.getY(), currentKoords.getX() + 2};
                    move_at(currentKoords);
                    refresh();
                }
                break;
            case ENTER:
                if (field_.empty()) {
                    field_generate(currentKoords);
                }
                if (open_cell(currentKoords) == MINE) {
                    addch(MINE);
                    sleep(sleepingTime);
                    ch = ESC;
                    screen_ = field_;
                    print_screen();
                    getch();
                }
                break;
            case FLAG:
                if (set_flag(currentKoords) == FLAG_VALUE)
                    ++flagsN;
                else
                    --flagsN;
                mvprintw(0, 0,"Number of mines = %i\nNumber of flags = %i", minesN_, flagsN);
                move_at(currentKoords);
                refresh();
                break;
            default:
                break;
        }
    } while (ch != ESC && ch != '');
    clear();

    return;
}

chtype Minefield::set_flag(const Koords &in) const
{
    Koords relativeKoords = koords_to_relative(in);
    if (screen_[relativeKoords.getY()][relativeKoords.getX()] == VOID_VALUE) {
        screen_[relativeKoords.getY()][relativeKoords.getX()] = FLAG_VALUE;
        print_value(relativeKoords.getY(), relativeKoords.getX());
        move_at(in);
    }
    else if (screen_[relativeKoords.getY()][relativeKoords.getX()] == FLAG_VALUE) {
        screen_[relativeKoords.getY()][relativeKoords.getX()] = VOID_VALUE;
        print_value(relativeKoords.getY(), relativeKoords.getX());
        move_at(in);
    }
    return screen_[relativeKoords.getY()][relativeKoords.getX()];
}

chtype Minefield::open_cell(const Koords &in) const
{
    Koords tempKoords = koords_to_relative(in);
    if (screen_[tempKoords.getY()][tempKoords.getX()] == VOID_VALUE) {
        screen_[tempKoords.getY()][tempKoords.getX()] = field_[tempKoords.getY()][tempKoords.getX()];
        print_value(tempKoords.getY(), tempKoords.getX());
        refresh();
        if (screen_[tempKoords.getY()][tempKoords.getX()] == DEF_VALUE) {
            for (int l{in.getY() - 1}; l < in.getY() + 2; ++l) {
                if (l > beginen_.getY() && l < end_.getY())
                    for (int m{in.getX() - 2}; m < in.getX() + 3; ++++m) {
                        if (m > beginen_.getX() && m < end_.getX()) {
                            move(l, m);
                            open_cell(Koords{l, m});
                        }
                    }
            }
        }
    }
    move_at(in);
    return screen_[tempKoords.getY()][tempKoords.getX()];
}

void Minefield::print_screen() const
{
    for (int i{}; i < screen_.size(); ++i) {
        move(beginen_.getY() + i + 1, beginen_.getX() + 1);
        for (int j{}; j < screen_[i].size(); ++j) {
            print_value(i, j);
        }
    }
    refresh();
    return;
}

void Minefield::print_value(const int i, const int j) const
{
    addch(screen_[i][j]);
    chtype free_cell{screen_[i][j] - static_cast<char>(screen_[i][j]) + ' '};
    addch(free_cell);
    return;
}
