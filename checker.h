#pragma once

// #include "cell.h"
#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>

struct Cell;

struct Checker : Graph_lib::Circle {
    //enum class Checker_color { Black, White};

    //Checker(win, Checker::white),
    //WhiteChecker(win);

    Checker(Graph_lib::Window& win) : Circle(Graph_lib::Point(0, 0), r) {
        win.attach(*this);
    }

    void draw_lines() const override {
        Circle::draw_lines();
    }

    void attach(const Cell& c);

    virtual bool is_black() const = 0;

    void detach() {
        cell = nullptr;
    }
private:
    static constexpr int r = 0.9 * (Cell::size) / 2;
    const Cell* cell{nullptr};
};

struct WhiteChecker : Checker {
    WhiteChecker(Graph_lib::Window& win) : Checker(win) {
        set_fill_color(Graph_lib::Color{17});
    }
    bool is_black() const override {
        return false;
    }
};

struct BlackChecker : Checker {
    BlackChecker(Graph_lib::Window& win) : Checker(win) {
        set_fill_color(Graph_lib::Color{30});
    }
    bool is_black() const override {
        return true;
    }
};