#pragma once

#include "figure.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <vector>
// #include "board.h"

using Graph_lib::Point;
using Graph_lib::Rectangle;
using Graph_lib::Shape;

struct Chessboard;

constexpr int c_size = 80;  // couldn't find a way to get this constant from "cell.h"

// Shape that represents a frame, that fits into a cell (80*80 pixels)
// Used when it's needed to highlight, that currently clicked figure can take another
struct Frame : Rectangle
{
    Frame(Point center, Chessboard& chess_);
    ~Frame();

    void draw_lines () const override;

  private:
    static constexpr int rc_width = 10;
    static constexpr int rc_length = 30;

    Chessboard* chess;

    std::vector<Rectangle*> horisontal_rectangles;  // Please check for
    std::vector<Rectangle*> vertical_rectangles;    // possible memory leaks
};                                                  // vector_ref possible?

// Stores all shapes that represent all possible moves for currently clicked figure
struct VisualSteps
{
    VisualSteps(Chessboard& chess_) : chess{&chess_} {}

    ~VisualSteps();
    std::vector<Frame*> possible_takes;
    std::vector<Graph_lib::Circle*> possible_steps;

  private:
    Chessboard* chess;
};