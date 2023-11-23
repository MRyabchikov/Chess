#pragma once
#include "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <iostream>

using Graph_lib::Address;
using Graph_lib::Point;

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
    Chessboard(Point xy);

    static constexpr int N = 8;
    static constexpr int N_max = 8;
    static_assert(N <= N_max, "do not allow board larger than N_max*N_max");

    Cell& at(char c, int i) {
        i--;
        int j = c - 'a';
        return cells[i*N + j];
    }
  private:
    static constexpr int margin = 30;
    static constexpr int width = N * Cell::size + 2 * margin + 70;
    static constexpr int height = N * Cell::size + 2 * margin;
    Graph_lib::Vector_ref<Cell> cells;

    static void cb_clicked (Address, Address widget)
    {
        auto& btn = Graph_lib::reference_to<Cell>(widget);
        dynamic_cast<Chessboard&>(btn.window()).clicked(btn);
    }

    void clicked (Cell& c);

    Graph_lib::Marks x_labels;
    Graph_lib::Marks y_labels;

    Cell* selected{nullptr};
};