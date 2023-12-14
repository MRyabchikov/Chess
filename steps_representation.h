#pragma once

#include "figure.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <vector>
// #include "board.h"

using Graph_lib::Vector_ref;
using Graph_lib::Circle;
using Graph_lib::Closed_polyline;
using Graph_lib::Point;
using Graph_lib::Rectangle;
using Graph_lib::Shape;

struct Chessboard;

constexpr int c_size = 80;

/*
template<typename T>
struct Resource_manager
{
    Resource_manager(Chessboard& chess_);
    ~Resource_manager(Chessboard& chess_);

    void push_back(T* shape);
    void push_back(T& shape);

    private:
        Vector_ref<T> shapes;
        Chessboard* chess;
};
*/

template<typename T>
struct Unique_attacher
{
    Unique_attacher(T* object_, Chessboard& chess_);
    Unique_attacher(const T&);
    Unique_attacher& operator=(const T&);
    ~Unique_attacher();
    T& obj();

    void draw_lines() const;
    void set_color(int color) const;
    void set_fill_color(int color) const;

    private:
        T* object;
        Chessboard* chess;
};



struct Detacher
{
    Detacher(Chessboard& chess_) : chess{&chess_} {}
    ~Detacher() {}

    void detach(Shape& fig);

    private:
        Chessboard* chess;
};

// Shape that represents a circle of circles, that fits into a cell (80*80 pixels)
// Used when it's needed to highlight a check
struct DangerSign : Circle
{
    DangerSign(Point center, Chessboard& chess_);
    ~DangerSign();

    void draw_lines() const override;

    private:
        static constexpr int circle_radius = 8;

        Detacher det;

        Vector_ref<Circle> circle_of_circles;
};

struct RedCross : Rectangle
{
    RedCross(Point center, Chessboard& chess_);
    ~RedCross();

    void draw_lines() const override;

    private:
        static constexpr int dist = 9;

        Detacher det;

        Vector_ref<Closed_polyline> rectangles;
};

// Shape that represents a frame, that fits into a cell (80*80 pixels)
// Used when it's needed to highlight, that currently clicked figure can take another
struct Frame : Rectangle
{
    Frame(Point center, Chessboard& chess_);
    // ~Frame();

    void draw_lines() const override;

    private:
        static constexpr int rc_width = 10;
        static constexpr int rc_length = 30;

        /*
        Detacher det;
        Vector_ref<Rectangle> horisontal_rectangles;
        Vector_ref<Rectangle> vertical_rectangles;
        */

       std::vector<Unique_attacher<Rectangle>> horisontal_rectangles;
       std::vector<Unique_attacher<Rectangle>> vertical_rectangles;
};

// Stores all shapes that represent all possible moves for currently clicked figure
struct VisualSteps
{
    VisualSteps(Chessboard& chess_) : det{chess_} {}

    ~VisualSteps();
    // Vector_ref<Frame> possible_takes;
    std::vector<Unique_attacher<Frame>> possible_takes;
    Vector_ref<Circle> possible_steps;
    Vector_ref<RedCross> disabled_steps;


    private:
        Detacher det;
};
