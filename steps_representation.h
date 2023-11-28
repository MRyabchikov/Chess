#pragma once


#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <vector>
#include "figure.h"
//#include "board.h"


using Graph_lib::Rectangle;
using Graph_lib::Point;
using Graph_lib::Shape;
using Graph_lib::Circle;
using Graph_lib::Closed_polyline;


struct Chessboard;


constexpr int c_size = 80;  //couldn't find a way to get this constant from "cell.h"

//Shape that represents a circle of circles, that fits into a cell (80*80 pixels)
//Used when it's needed to highlight a check
struct DangerSign : Circle
{
    DangerSign(Point center, Chessboard& chess_);
    ~DangerSign();

    void draw_lines() const override;

    private:
        static constexpr int circle_radius = 8;

        Chessboard *chess;

        std::vector<Circle*> circle_of_circles; //Please check for possible memory leaks
};

struct RedCross : Rectangle
{
    RedCross(Point center, Chessboard& chess_);
    ~RedCross();

    void draw_lines() const override;

    private:
        static constexpr int dist = 9;

        Chessboard *chess;

        Closed_polyline* rectangle_1;
        Closed_polyline* rectangle_2;
};

//Shape that represents a frame, that fits into a cell (80*80 pixels)
//Used when it's needed to highlight, that currently clicked figure can take another
struct Frame : Rectangle
{
    Frame(Point center, Chessboard& chess_);
    ~Frame();
    
    void draw_lines() const override;

    private:
        static constexpr int rc_width = 10;
        static constexpr int rc_length = 30;

        Chessboard* chess;

        std::vector<Rectangle*> horisontal_rectangles;  //Please check for 
        std::vector<Rectangle*> vertical_rectangles;    //possible memory leaks
};


//Stores all shapes that represent all possible moves for currently clicked figure
struct VisualSteps
{
    VisualSteps(Chessboard& chess_) : chess{&chess_} {}
    ~VisualSteps();
    std::vector<Frame*> possible_takes;
    std::vector<Graph_lib::Circle*> possible_steps;

    private:
        Chessboard* chess;
};