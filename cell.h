#pragma once
#include "figure.h"
//#include "board.h"

#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

constexpr int DFTBOF = 30; //distance_from_the_beginning_of_coordinates

struct Checker;

struct Coordinate
{
    Coordinate(int x_, int y_): x{x_}, y{y_} {}
    int x;
    int y;
};

using Graph_lib::Point;

struct Cell : Graph_lib::Button
{
    enum Type
    {
        black,
        white,
        dark_green
    };

    Cell(Point xy, Graph_lib::Callback cb, Type t);

    void attach (Graph_lib::Window& win) override;

    void activate ()
    {
        if (pw)
            pw->color(FL_SELECTION_COLOR);
    }

    void deactivate () { reset_color(); }

    bool is_black () const { return type == dark_green; }

    static constexpr int size = 80;

    Point center () const { return Point{loc.x + width / 2, loc.y + height / 2}; }

    void attach_figure (Figure& ch);

    Figure& detach_figure ();

    bool has_figure () const { return figure != nullptr; }

    /*const*/ Figure& get_figure () const;  // обязательно нужна проверка не нулевой ли указатель checker
                                            // removed const and i don't know how it will turn out

    Coordinate location(); // получаем пару {x, y} - координату положения клетки

  private:
    Type type;
    void reset_color ();
    Figure* figure{nullptr};
};
