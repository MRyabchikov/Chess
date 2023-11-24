#pragma once

// "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

struct Cell;

struct Figure : Graph_lib::Image
{  // Graph_lib::Image

    enum Type
    {
        black,
        white,
    };

    Figure(Graph_lib::Window& win, std::string path_to_image) : Graph_lib::Image(Graph_lib::Point(0, 0), path_to_image)
    {
        win.attach(*this);
    }

    void draw_lines () const override { Graph_lib::Image::draw_lines(); }

    void attach (const Cell& c);

    bool is_black () { return color == Type::black; }

    bool is_white () { return color == Type::white; }

    void detach () { cell = nullptr; }

  private:
    bool color;
    static constexpr int r = 0.9 * (80) / 2;
    const Cell* cell{nullptr};
};

struct Pawn : Figure
{
    Pawn(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wP.png" : "bP.png"){};
};

struct King : Figure
{
    King(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wK.png" : "bK.png"){};
};

struct Bishop : Figure
{
    Bishop(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wB.png" : "bB.png"){};
};

struct Knight : Figure
{
    Knight(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wN.png" : "bN.png"){};
};

struct Queen : Figure
{
    Queen(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wQ.png" : "bQ.png"){};
};

struct Rook : Figure
{
    Rook(Graph_lib::Window& win, bool color) : Figure(win, color == Type::white ? "wR.png" : "bR.png"){};
};

// struct WhiteChecker : Checker {
// WhiteChecker(Graph_lib::Window& win) : Checker(win) {
//     set_fill_color(Graph_lib::Color{17});
// }
// bool is_black() const override {
//     return false;
// }
// };

// struct BlackChecker : Checker {
//     BlackChecker(Graph_lib::Window& win) : Checker(win) {
//         set_fill_color(Graph_lib::Color{30});
//     }
//     bool is_black() const override {
//         return true;
//     }
// };