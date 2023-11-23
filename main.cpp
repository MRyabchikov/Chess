#include "board.h"
#include "cell.h"
// #include "checker.h"
#include "pawn.h"

int main ()
{
    Chessboard chess({200, 200});

    // белые пешки
    Form wp0(chess, "wP.png"), wp1(chess, "wP.png"), wp2(chess, "wP.png"), wp3(chess, "wP.png"), wp4(chess, "wP.png"), wp5(chess, "wP.png"), 
        wp6(chess, "wP.png"), wp7(chess, "wP.png");

    chess.at('a', 2).attach_form(wp0);
    chess.at('b', 2).attach_form(wp1);
    chess.at('c', 2).attach_form(wp2);
    chess.at('d', 2).attach_form(wp3);
    chess.at('e', 2).attach_form(wp4);
    chess.at('f', 2).attach_form(wp5);
    chess.at('g', 2).attach_form(wp6);
    chess.at('h', 2).attach_form(wp7);

    // черные пешки
    Form bp0(chess, "bP.png"), bp1(chess, "bP.png"), bp2(chess, "bP.png"), bp3(chess, "bP.png"), bp4(chess, "bP.png"), bp5(chess, "bP.png"), 
        bp6(chess, "bP.png"), bp7(chess, "bP.png");

    chess.at('a', 7).attach_form(bp0);
    chess.at('b', 7).attach_form(bp1);
    chess.at('c', 7).attach_form(bp2);
    chess.at('d', 7).attach_form(bp3);
    chess.at('e', 7).attach_form(bp4);
    chess.at('f', 7).attach_form(bp5);
    chess.at('g', 7).attach_form(bp6);
    chess.at('h', 7).attach_form(bp7);

    // белые кони
    Form wn0(chess, "wN.png"), wn1(chess, "wN.png");

    chess.at('b', 1).attach_form(wn0);
    chess.at('g', 1).attach_form(wn1);

    // черыне кони
    Form bn0(chess, "bN.png"), bn1(chess, "bN.png");

    chess.at('b', 8).attach_form(bn0);
    chess.at('g', 8).attach_form(bn1);

    // белые слоны
    Form wb0(chess, "wB.png"), wb1(chess, "wB.png");

    chess.at('c', 1).attach_form(wb0);
    chess.at('f', 1).attach_form(wb1);

    // черные слоны
    Form bb0(chess, "bB.png"), bb1(chess, "bB.png");

    chess.at('c', 8).attach_form(bb0);
    chess.at('f', 8).attach_form(bb1);

    // белые ладьи
    Form wr0(chess, "wR.png"), wr1(chess, "wR.png");

    chess.at('a', 1).attach_form(wr0);
    chess.at('h', 1).attach_form(wr1);

    // черные ладьи
    Form br0(chess, "bR.png"), br1(chess, "bR.png");

    chess.at('a', 8).attach_form(br0);
    chess.at('h', 8).attach_form(br1);

    // Белый король
    Form wk(chess, "wK.png");

    chess.at('e', 1).attach_form(wk);

    // Черный король
    Form bk(chess, "bK.png");

    chess.at('e', 8).attach_form(bk);

    // Белый ферзь
    Form wq(chess, "wQ.png");

    chess.at('d', 1).attach_form(wq);

    // Черный ферщь
    Form bq(chess, "bQ.png");

    chess.at('d', 8).attach_form(bq);


    chess.wait_for_button();
}

//C:/Projects/build/Chessboard2/chessboard.exe