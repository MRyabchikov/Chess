#pragma once

// "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <vector>
#include <memory>

using Graph_lib::Rectangle;
using Graph_lib::Point;

constexpr int a_ascii = 97;
constexpr int c_size = 80; //couldn't find a way to get this constant from "cell.h"

struct Cell;
struct Chessboard;
struct Coordinate;

struct Frame : Rectangle
{
    Frame(Point center);
    ~Frame();
    
    void draw_lines() const override;

    private:
        static constexpr int rc_width = 10;
        static constexpr int rc_length = 30;

        std::vector<Rectangle*> horisontal_rectangles; //Please check for 
        std::vector<Rectangle*> vertical_rectangles;   //possible memory leaks
};

struct VisualSteps
{
    ~VisualSteps();
    std::vector<Frame*> possible_takes;
    std::vector<Graph_lib::Circle*> possible_steps;
};

struct Figure : Graph_lib::Image
{  // Graph_lib::Image

    enum Type
    {
        black,
        white,
    };

    Figure(Graph_lib::Window& win, Type color, std::string path_to_image) : Graph_lib::Image(Graph_lib::Point(0, 0), path_to_image)
    {
        this->color = color;
        win.attach(*this);
    }

    virtual bool correct_step(Cell& c1, Cell& c2) = 0;
    virtual VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) = 0;

    void draw_lines () const override { Graph_lib::Image::draw_lines(); }

    void attach (const Cell& c);

    bool is_white () { return color == white; }

    bool is_black () { return color == black; }

    void detach () { cell = nullptr; }

  private:
    bool color;
    static constexpr int r = 0.9 * (80) / 2;
    const Cell* cell{nullptr};
};

struct Pawn : Figure
{
    Pawn(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wP.png" : "bP.png"),
                                               first_step{true} {}

    bool correct_step(Cell& c1, Cell& c2) override;
    VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) override;

    private:
        bool first_step;
};

struct King : Figure
{
    King(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wK.png" : "bK.png"){};
};

struct Bishop : Figure
{
    Bishop(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wB.png" : "bB.png"){};
};

struct Knight : Figure
{
    Knight(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wN.png" : "bN.png"){};
};

struct Queen : Figure
{
    Queen(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wQ.png" : "bQ.png"){};
};

struct Rook : Figure
{
    Rook(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wR.png" : "bR.png"){};
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