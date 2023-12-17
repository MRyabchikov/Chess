#pragma once

#include "figure.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include "attach_manager.t.hpp"
#include <vector>

using Graph_lib::Vector_ref;
using Graph_lib::Circle;
using Graph_lib::Closed_polyline;
using Graph_lib::Point;
using Graph_lib::Rectangle;
using Graph_lib::Shape;

struct Simple_window;

constexpr int c_size = 80;

// Shape that represents a circle of circles, that fits into a cell (80*80 pixels)
// Used when it's needed to highlight a check
struct DangerSign : Circle
{
    DangerSign(Point center, Simple_window& chess_);

    void draw_lines () const override;

    private:
        static constexpr int circle_radius = 8;

        std::vector<AttachManager<Circle>> circle_of_circles;
};

struct RedCross : Rectangle
{
    RedCross(Point center, Simple_window& chess_);

    void draw_lines () const override;

    private:
        static constexpr int dist = 13;

        std::vector<AttachManager<Closed_polyline>> rectangles;
};

// Shape that represents a frame, that fits into a cell (80*80 pixels)
// Used when it's needed to highlight, that currently clicked figure can take another
struct Frame : Rectangle
{
    Frame(Point center, Simple_window& chess_);

    void draw_lines () const override;

    private:
        static constexpr int rc_width = 10;
        static constexpr int rc_length = 30;

        std::vector<AttachManager<Rectangle>> horisontal_rectangles;
        std::vector<AttachManager<Rectangle>> vertical_rectangles;
};

// Stores all shapes that represent all possible moves for currently clicked figure
struct VisualSteps
{
    std::vector<AttachManager<Frame>> possible_takes;
    std::vector<AttachManager<Circle>> possible_steps;
    std::vector<AttachManager<RedCross>> disabled_steps;
};
