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
    Pawn wp0(chess, Figure::Type::white), wp1(chess, Figure::Type::white),
         wp2(chess, Figure::Type::white), wp3(chess, Figure::Type::white),
         wp4(chess, Figure::Type::white), wp5(chess, Figure::Type::white),
         wp6(chess, Figure::Type::white), wp7(chess, Figure::Type::white);

    chess.at('a', 2).attach_figure(wp0);
    chess.at('b', 2).attach_figure(wp1);
    chess.at('c', 2).attach_figure(wp2);
    chess.at('d', 2).attach_figure(wp3);
    chess.at('e', 2).attach_figure(wp4);
    chess.at('f', 2).attach_figure(wp5);
    chess.at('g', 2).attach_figure(wp6);
    chess.at('h', 2).attach_figure(wp7);

    // черные пешки
    Pawn bp0(chess, Figure::Type::black), bp1(chess, Figure::Type::black),
         bp2(chess, Figure::Type::black), bp3(chess, Figure::Type::black),
         bp4(chess, Figure::Type::black), bp5(chess, Figure::Type::black),
         bp6(chess, Figure::Type::black), bp7(chess, Figure::Type::black);

    chess.at('a', 7).attach_figure(bp0);
    chess.at('b', 7).attach_figure(bp1);
    chess.at('c', 7).attach_figure(bp2);
    chess.at('d', 7).attach_figure(bp3);
    chess.at('e', 7).attach_figure(bp4);
    chess.at('f', 7).attach_figure(bp5);
    chess.at('g', 7).attach_figure(bp6);
    chess.at('h', 7).attach_figure(bp7);


    /*

    // белые кони
    Knight wn0(chess, Figure::Type::white), wn1(chess, Figure::Type::white);

    chess.at('b', 1).attach_figure(wn0);
    chess.at('g', 1).attach_figure(wn1);

    // черыне кони
    Knight bn0(chess, Figure::Type::black), bn1(chess, Figure::Type::black);

    chess.at('b', 8).attach_figure(bn0);
    chess.at('g', 8).attach_figure(bn1);

    // белые слоны
    Bishop wb0(chess, Figure::Type::white), wb1(chess, Figure::Type::white);

    chess.at('c', 1).attach_figure(wb0);
    chess.at('f', 1).attach_figure(wb1);

    // черные слоны
    Bishop bb0(chess, Figure::Type::black), bb1(chess, Figure::Type::black);

    chess.at('c', 8).attach_figure(bb0);
    chess.at('f', 8).attach_figure(bb1);

    // белые ладьи
    Rook wr0(chess, Figure::Type::white), wr1(chess, Figure::Type::white);

    chess.at('a', 1).attach_figure(wr0);
    chess.at('h', 1).attach_figure(wr1);

    // черные ладьи
    Rook br0(chess, Figure::Type::black), br1(chess, Figure::Type::black);

    chess.at('a', 8).attach_figure(br0);
    chess.at('h', 8).attach_figure(br1);

    // Белый король
    King wk(chess, Figure::Type::white);

    chess.at('e', 1).attach_figure(wk);

    // Черный король
    King bk(chess, Figure::Type::black);

    chess.at('e', 8).attach_figure(bk);

    // Белый ферзь
    Queen wq(chess, Figure::Type::white);

    chess.at('d', 1).attach_figure(wq);

    // Черный ферщь
    Queen bq(chess, Figure::Type::black);

    chess.at('d', 8).attach_figure(bq);

    */

    /*
    std::cout << chess.at('a',1).location().x << ' ' << chess.at('a',1).location().y << '\n';
    std::cout << chess.at('b',1).location().x << ' ' << chess.at('b',1).location().y << '\n';
    std::cout << chess.at('a',8).location().x << ' ' << chess.at('a',8).location().y << '\n';
    std::cout << chess.at('h',8).location().x << ' ' << chess.at('h',8).location().y << '\n';
    std::cout << chess.at('a',7).location().x << ' ' << chess.at('a',7).location().y << '\n';
    std::cout << chess.at('a',5).location().x << ' ' << chess.at('a',5).location().y << '\n';
    */

    //Frame frm{chess.at('a',8).center()};
    //chess.attach(frm);

    //Graph_lib::Rectangle rec{{200,200}, 80, 80};
    //rec.set_fill_color(Graph_lib::Color::green);
    //chess.attach(rec);

    std::cout << chess.at('a',8).center().x << ' ' << chess.at('a',8).center().y << '\n';

    chess.wait_for_button();

    return 0;
}

// C:/Projects/build/Chessboard2/chessboard.exe