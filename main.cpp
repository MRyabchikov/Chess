#include "board.h"
#include "cell.h"
// #include "checker.h"
#include "figure.h"
#include <iostream>

extern const Graph_lib::Point Chessboard_location;

int main()
{
    Chessboard chess{Chessboard_location};
    chess.standard_fill();

    Circle* tempc = new Circle{{400,400},200};
    tempc->set_color(Graph_lib::Color::red);
    tempc->set_fill_color(Graph_lib::Color::red);
    Unique_attacher<Circle> ua_tempc{tempc, chess};

    chess.wait_for_button();
    return 0;
}
