#include "cell.h"
//
#include "figure.h"
// #include "pawn.h"

using namespace Graph_lib;

Cell::Cell(Point xy, Callback cb, Type t) : Button{xy, size, size, "", cb}, type{t} {}

void Cell::reset_color()
{
    if (!pw)
        error("Cell is not attached to a window");
    if (is_black())
        pw->color(Color::black);
    else
        pw->color(Color::white);
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

const Figure& Cell::get_figure() const
{  // обязательно нужна проверка не нулевой ли указатель checker
    return *figure;
}