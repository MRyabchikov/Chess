#pragma once
#include "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <iostream>

using Graph_lib::Address;
using Graph_lib::Point;
using Graph_lib::Vector_ref;

const Graph_lib::Point Chessboard_location{200,200};

//Class, that doesn't allow you to shoot yourself in the foot while
//trying to get a "Cell" value using indices
class Sub_Vector_ref
{
  public:
    Sub_Vector_ref() = default;
    Sub_Vector_ref(Graph_lib::Vector_ref<Cell> v_):  v{v_} {}

    Cell& operator[](int i);

  private:
    Graph_lib::Vector_ref<Cell> v;
};

class MyWindow : public Simple_window
{
  public:
    MyWindow(Point xy, int w, int h, const std::string& title);

  private:
    Graph_lib::Button quit_button;
    static void cb_quit (Address, Address widget);

    void quit () { hide(); }
};

Cell::Type type_of_cell (int i, int j);

struct Chessboard : MyWindow
{

    enum step_color
    {
        white,
        black
    };

    Chessboard(Point xy);

    void standard_fill();

    static constexpr int N = 8;
    static constexpr int N_max = 8;
    static_assert(N <= N_max, "do not allow board larger than N_max*N_max");
    //return a 1D array of values of a column of a 'c' coordinate
    Sub_Vector_ref operator[](char c);

    //says for itself
    Chessboard* deepcopy();
    //friend Chessboard* Chessboard::deepcopy();

    // friend VisualSteps* Figure::show_possible_steps(Coordinate position, Chessboard& chess);

    bool out_of_range (Coordinate pos);

    bool is_stalemate (step_color stalemate_candidate) { return true; }

  private:
    static constexpr int margin = 30;
    static constexpr int width = N * Cell::size + 2 * margin + 70;
    static constexpr int height = N * Cell::size + 2 * margin;

    step_color step_chooser;

    Vector_ref<Cell> cells;

    //these are made so they don't get out of scope
    Vector_ref<Pawn> pawns;
    Vector_ref<Rook> rooks;
    Vector_ref<Knight> knights;
    Vector_ref<Bishop> bishops;
    Vector_ref<Queen> queens;
    Vector_ref<King> kings;



    static void cb_clicked (Address, Address widget)
    {
        auto& btn = Graph_lib::reference_to<Cell>(widget);
        dynamic_cast<Chessboard&>(btn.window()).clicked(btn);
    }

    Cell& at(char c, int i)
    {
        i--;
        int j = c - 'a';
        return cells[i*N + j];
    }

    void clicked (Cell& c);

    bool decide ();

    void step_swap () { step_chooser = (step_chooser == step_color::white) ? step_color::black : step_color::white; }

    void reset_double_steps();

    Graph_lib::Marks x_labels;
    Graph_lib::Marks y_labels;

    Cell* selected{nullptr};

    VisualSteps* all_possible_steps;
};
