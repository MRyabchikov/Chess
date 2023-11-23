#include "cell.h"
//
#include "pawn.h"
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

Form& Cell::detach_form()
{
    Form* ch = form;
    form = nullptr;
    ch->detach();
    return *ch;
}

void Cell::attach_form(Form& ch)
{
    ch.attach(*this);
    form = &ch;
}

const Form& Cell::get_form() const
{  // обязательно нужна проверка не нулевой ли указатель checker
    return *form;
}