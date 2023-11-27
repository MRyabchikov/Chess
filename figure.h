#pragma once

// "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include "steps_representation.h"

constexpr int a_ascii = 97; //ascii code of letter 'a'


struct Cell;
struct Chessboard;
struct Coordinate;
struct VisualSteps;


struct Figure : Graph_lib::Image
{  // Graph_lib::Image

    enum Type
    {
        black,
        white
    };

    Figure(Graph_lib::Window& win, Type color, std::string path_to_image) : Graph_lib::Image(Graph_lib::Point(0, 0), path_to_image)
    {
        this->color = color;
        win.attach(*this);
    }

    //Checks if clicked cell satisfies the conditions of a correct move
    virtual bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) = 0;

    //Creates an object "VisualSteps" that is required to show all possible moves of currently clicked figure
    virtual VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) = 0;

    bool change_pos_decider(Cell& c);
                                                                                
    void draw_lines () const override { Graph_lib::Image::draw_lines(); }

    void attach (const Cell& c);

    bool is_white () const { return color == white; }

    bool is_black () const { return color == black; }

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

    bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) override;
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

    bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) override;
    VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) override;

    private:
        //HF - help function
        void show_possible_steps_HF(int x, int y, int x0, int y0, int d1, int d2,
                                    VisualSteps*& steps_representation, Chessboard& chess);
};

struct Knight : Figure
{
    Knight(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wN.png" : "bN.png"){};

    bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) override;
    VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) override;
};

struct Queen : Figure
{
    Queen(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wQ.png" : "bQ.png"){};

    bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) override;
    VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) override;

    private:
        void horisontal_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation);
        void vertical_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation);
        void diagnal_possible_steps(int x, int y, int x0, int y0, Coordinate& position,
                                    Chessboard& chess, VisualSteps* & steps_representation);
        void show_possible_steps_HF(int x, int y, int x0, int y0, int d1, int d2,
                                    VisualSteps*& steps_representation, Chessboard& chess);
};

struct Rook : Figure
{
    Rook(Graph_lib::Window& win, Figure::Type color) : Figure(win, color, color == Type::white ? "wR.png" : "bR.png"){};

    bool correct_step(Cell& c1, Cell& c2, Chessboard& chess) override;
    VisualSteps* show_possible_steps(Coordinate position, Chessboard& chess) override;

    private:
        void horisontal_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation);
        void vertical_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation);
};