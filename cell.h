#pragma once
#include "pawn.h"

#include <Graph_lib/GUI.h>
#include <Graph_lib/Graph.h>

struct Checker;

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

    void attach(Graph_lib::Window& win) override;

    void activate() {if(pw) pw -> color(FL_SELECTION_COLOR); }
    void deactivate() { reset_color();}

    bool is_black () const { return type == dark_green; }

    static constexpr int size = 80;

    Point center() const { 
        return Point{loc.x + width/2, loc.y + height/2}; 
    }

    void attach_form(Form& ch);

    Form& detach_form();

    bool has_form() const {
        
        return form != nullptr;
    }

    const Form& get_form() const; // обязательно нужна проверка не нулевой ли указатель checker
  private:
    Type type;
    void reset_color ();
    Form* form{nullptr};
};

