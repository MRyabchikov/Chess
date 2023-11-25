#include "cell.h"
//
#include "figure.h"
#include <iostream>
// #include "pawn.h"

//extern const Graph_lib::Point Chessboard_location;

using namespace Graph_lib;

Cell::Cell(Point xy, Callback cb, Type t) : Button{xy, size, size, "", cb}, type{t} {}

void Cell::reset_color()
{
    if (!pw)
        error("Cell is not attached to a window");
    if (is_black())
        pw->color(/*Color::chess_green*/Color::black);
    else
        pw->color(/*Color::chess_white*/Color::white);
}

void Cell::attach(Graph_lib::Window& win)
{
    Button::attach(win);
    reset_color();
}

Figure& Cell::detach_figure()
{
    Figure* ch = figure;
    figure = nullptr;
    ch->detach();
    return *ch;
}

void Cell::attach_figure(Figure& ch)
{
    ch.attach(*this);
    figure = &ch;
}

/*const*/ Figure& Cell::get_figure() const                         //removed const and i don't know
{  // обязательно нужна проверка не нулевой ли указатель checker   //how it will turn out
    return *figure;
}

Coordinate Cell::location()
{

    int N = 8;                          // Couldn't find a way to properly use
                                        //the static constant from "board.h"

    int x = (loc.x - DFTBOF)/size + 1;
    int y = N - (loc.y - DFTBOF)/size;

    return Coordinate{x,y};
}