#include "board.h"
#include "cell.h"
// #include "checker.h"
#include "figure.h"
#include <iostream>

extern const Graph_lib::Point Chessboard_location;

int main ()
{
    Chessboard chess(Chessboard_location);

    // белые пешки
    Pawn wp0(chess, Figure::Type::white), wp1(chess, Figure::Type::white), wp2(chess, Figure::Type::white),
        wp3(chess, Figure::Type::white), wp4(chess, Figure::Type::white), wp5(chess, Figure::Type::white),
        wp6(chess, Figure::Type::white), wp7(chess, Figure::Type::white);

    chess['a'][2].attach_figure(wp0);
    chess['b'][2].attach_figure(wp1);
    chess['c'][2].attach_figure(wp2);
    chess['d'][2].attach_figure(wp3);
    chess['e'][2].attach_figure(wp4);
    chess['f'][2].attach_figure(wp5);
    chess['g'][2].attach_figure(wp6);
    chess['h'][2].attach_figure(wp7);

    // черные пешки
    Pawn bp0(chess, Figure::Type::black), bp1(chess, Figure::Type::black), bp2(chess, Figure::Type::black),
        bp3(chess, Figure::Type::black), bp4(chess, Figure::Type::black), bp5(chess, Figure::Type::black),
        bp6(chess, Figure::Type::black), bp7(chess, Figure::Type::black);

    chess['a'][7].attach_figure(bp0);
    chess['b'][7].attach_figure(bp1);
    chess['c'][7].attach_figure(bp2);
    chess['d'][7].attach_figure(bp3);
    chess['e'][7].attach_figure(bp4);
    chess['f'][7].attach_figure(bp5);
    chess['g'][7].attach_figure(bp6);
    chess['h'][7].attach_figure(bp7);

    // белые кони
    Knight wn0(chess, Figure::Type::white), wn1(chess, Figure::Type::white);

    chess['b'][1].attach_figure(wn0);
    chess['g'][1].attach_figure(wn1);

    // черыне кони
    Knight bn0(chess, Figure::Type::black), bn1(chess, Figure::Type::black);

    chess['b'][8].attach_figure(bn0);
    chess['g'][8].attach_figure(bn1);

    // белые слоны
    Bishop wb0(chess, Figure::Type::white), wb1(chess, Figure::Type::white);

    chess['c'][1].attach_figure(wb0);
    chess['f'][1].attach_figure(wb1);

    // черные слоны
    Bishop bb0(chess, Figure::Type::black), bb1(chess, Figure::Type::black);

    chess['c'][8].attach_figure(bb0);
    chess['f'][8].attach_figure(bb1);

    // белые ладьи
    Rook wr0(chess, Figure::Type::white), wr1(chess, Figure::Type::white);

    chess['a'][1].attach_figure(wr0);
    chess['h'][1].attach_figure(wr1);

    // черные ладьи
    Rook br0(chess, Figure::Type::black), br1(chess, Figure::Type::black);

    chess['a'][8].attach_figure(br0);
    chess['h'][8].attach_figure(br1);

    // Белый король
    King wk(chess, Figure::Type::white);

    chess['e'][1].attach_figure(wk);

    // Черный король
    King bk(chess, Figure::Type::black);

    chess['e'][8].attach_figure(bk);

    // Белый ферзь
    Queen wq(chess, Figure::Type::white);

    chess['d'][1].attach_figure(wq);

    // Черный ферзь
    Queen bq(chess, Figure::Type::black);

    chess['d'][8].attach_figure(bq);

    chess.wait_for_button();

    return 0;
}

// C:/Projects/build/Chessboard2/chessboard.exe