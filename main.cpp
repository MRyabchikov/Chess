#include "board.h"
#include "cell.h"
// #include "checker.h"
#include "figure.h"
#include <iostream>

extern const Graph_lib::Point Chessboard_location;

int main ()
{
    Chessboard chess{Chessboard_location};
    chess.standard_fill();
    chess.wait_for_button();
    return 0;
}
