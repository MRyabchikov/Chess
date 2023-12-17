#include "board.h"
#include "cell.h"
#include <iostream>

Cell& Sub_Vector_ref::operator[](int i)
{
    if(i < 1 || i > 8)
        Graph_lib::error("Out of range: vertical");
    return v[i-1];
}

MyWindow::MyWindow(Point xy, int w, int h, const std::string& title)
    : Simple_window{xy, w, h, title}, quit_button{Point{x_max() - 70, 20}, 70, 20, "Quit", cb_quit}
{
    attach(quit_button);
}

void MyWindow::cb_quit(Address, Address widget)
{   // void*
    auto& btn = Graph_lib::reference_to<Graph_lib::Button>(widget);
    dynamic_cast<MyWindow&>(btn.window()).quit();
}

Cell::Type type_of_cell (int i, int j)
{
    if (i % 2 == 0)
        return (j % 2 == 0) ? Cell::dark_green : Cell::white;
    else
        return (j % 2 == 0) ? Cell::white : Cell::dark_green;
}

std::string letters ()
{
    std::string s(Chessboard::N_max, '\0');
    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = 'a' + i;
    }
    return s;
}

std::string digits ()
{
    std::string s(Chessboard::N_max, '\0');

    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = '1' + i;
    }
    return s;
}

Chessboard::Chessboard(Point xy) : MyWindow{xy, width, height, "Chessboard"}, x_labels{letters()}, y_labels{digits()}
{
    size_range(width, height, width, height);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cells.push_back(new Cell{Point{margin + j * Cell::size, margin + (N - 1 - i) * Cell::size}, cb_clicked,
                                     type_of_cell(i, j)});
            attach(cells[cells.size() - 1]);
        }
    }

    for (int i = 0; i < N; ++i)
    {
        // board corners: left bottom
        constexpr Point lb{margin + Cell::size / 2, margin + N * Cell::size + 10};
        // and right bottom
        constexpr Point rb{margin - 10, margin + N * Cell::size - Cell::size / 2};
        x_labels.add(Point{lb.x + i * Cell::size, lb.y});
        y_labels.add(Point{rb.x, rb.y - i * Cell::size});
    }
    attach(x_labels);
    attach(y_labels);

    all_possible_steps = nullptr;

    step_chooser = step_color::white;
}

void Chessboard::standard_fill()
{
    // белые пешки
    for(int i = 0; i < 8; i++)
    {
        Pawn* temp_pawn = new Pawn(*this, Figure::Type::white);
        pawns.push_back(temp_pawn);
        at(char(a_ascii+i),2).attach_figure(pawns[i]);
    }

    // чёрные пешки
    for(int i = 8; i < 16; i++)
    {
        Pawn* temp_pawn = new Pawn(*this, Figure::Type::black);
        pawns.push_back(temp_pawn);
        at(char(a_ascii+i%8),7).attach_figure(pawns[i]);
    }


    // белые кони
    Knight* wn0 = new Knight{*this, Figure::Type::white};
    Knight* wn1 = new Knight{*this, Figure::Type::white};

    knights.push_back(wn0);
    knights.push_back(wn1);

    at('b',1).attach_figure(knights[0]);
    at('g',1).attach_figure(knights[1]);

    // черыне кони
    Knight* bn0 = new Knight{*this, Figure::Type::black};
    Knight* bn1 = new Knight{*this, Figure::Type::black};

    knights.push_back(bn0);
    knights.push_back(bn1);

    at('b',8).attach_figure(knights[2]);
    at('g',8).attach_figure(knights[3]);

    // белые слоны
    Bishop* wb0 = new Bishop{*this, Figure::Type::white};
    Bishop* wb1 = new Bishop{*this, Figure::Type::white};

    bishops.push_back(wb0);
    bishops.push_back(wb1);

    at('c',1).attach_figure(bishops[0]);
    at('f',1).attach_figure(bishops[1]);

    // черные слоны
    Bishop* bb0 = new Bishop{*this, Figure::Type::black};
    Bishop* bb1 = new Bishop{*this, Figure::Type::black};

    bishops.push_back(bb0);
    bishops.push_back(bb1);

    at('c',8).attach_figure(bishops[2]);
    at('f',8).attach_figure(bishops[3]);

    // белые ладьи
    Rook* wr0 = new Rook{*this, Figure::Type::white};
    Rook* wr1 = new Rook{*this, Figure::Type::white};

    rooks.push_back(wr0);
    rooks.push_back(wr1);

    at('a',1).attach_figure(rooks[0]);
    at('h',1).attach_figure(rooks[1]);

    // черные ладьи
    Rook* br0 = new Rook{*this, Figure::Type::black};
    Rook* br1 = new Rook{*this, Figure::Type::black};

    rooks.push_back(br0);
    rooks.push_back(br1);

    at('a',8).attach_figure(rooks[2]);
    at('h',8).attach_figure(rooks[3]);

    // Белый король
    King* wk = new King{*this, Figure::Type::white};

    kings.push_back(wk);

    at('e',1).attach_figure(kings[0]);

    // Черный король
    King* bk = new King{*this, Figure::Type::black};

    kings.push_back(bk);

    at('e',8).attach_figure(kings[1]);

    // Белый ферзь
    Queen* wq = new Queen{*this, Figure::Type::white};

    queens.push_back(wq);

    at('d',1).attach_figure(queens[0]);

    // Черный ферзь
    Queen* bq = new Queen{*this, Figure::Type::black};

    queens.push_back(bq);

    at('d',8).attach_figure(queens[1]);
}

void Chessboard::clicked(Cell& c)
{   // для шашек. Для всего остального удалить
    // if (!c.is_black()) return; для контроля, кто должен ходить
    //  в c также лежит информация о том, стоит ли фигура на это клетке. (предположительно)
    //  если стоит, то has_checker() == true;
    // std::cout << "*\n";
    // std::cout << "1\n";
    if (!selected)
    {
        // all_possible_steps = nullptr;
        selected = &c;
        c.activate();  // подсвечивает
        if (decide() == false)
        {
            c.deactivate();
            selected = nullptr;
            return;
        }
        // Create visual representation of moves for current figure
        all_possible_steps = c.get_figure().show_possible_steps(c.location(), *this);
        // std::cout << "2\n";
    }
    else
    {
        // std::cout << "3\n";
        if (selected->has_figure())
        {
            int a = selected->get_figure().correct_step(*selected, c, *this);
            if (a > 0)
            {
                // if()
                //  move_figure
                Cell& c1 = *selected;
                int x = c.location().x, y = c.location().y;

                int b;
                if (step_chooser == black)
                    b = 1;
                else if(step_chooser == white)
                    b = -1;
                if (c.has_figure())
                {
                    // taking the figure from the opponent
                    detach(c.detach_figure());            // убираем фигуру врага
                    c.attach_figure(c1.detach_figure());  // переносим свою
                }
                else if (1 <= y + b && y + b <= 8 && (*this)[x][y + b].has_figure() && (a == 2 || a == 3))
                {
                    detach((*this)[x][y + b].detach_figure());  // *this = chess
                    (*this)[x][y].attach_figure(c1.detach_figure());
                }
                else if(c1.get_figure().is_king())
                {
                    King* king_ptr = dynamic_cast<King*>(&c1.get_figure());

                    std::array<bool, 4> temp_arr = king_ptr->castlings();

                    int x1 = c1.location().x;
                    int y1 = c1.location().y;

                    if(temp_arr[0])
                        (*this)[x1-1][y1].attach_figure((*this)[x1-3][y1].detach_figure());
                    else if(temp_arr[1])
                        (*this)[x1-1][y1].attach_figure((*this)[x1-4][y1].detach_figure());
                    else if(temp_arr[2])
                        (*this)[x1+1][y1].attach_figure((*this)[x1+3][y1].detach_figure());
                    else if(temp_arr[3])
                        (*this)[x1+1][y1].attach_figure((*this)[x1+4][y1].detach_figure());

                    c.attach_figure(c1.detach_figure());
                }
                else
                {
                    c.attach_figure(c1.detach_figure());
                }

                step_swap();
                reset_double_steps();

                
                if(!is_check() && !is_mate())
                {
                    if(am_check_sign != nullptr)
                    {
                        delete am_check_sign;
                        am_check_sign = nullptr;
                    }
                }
                if(is_check() && !is_mate())
                {
                    if(am_check_sign == nullptr)
                    {
                        DangerSign* check_sign = new DangerSign{find_king(step_chooser)->center(), *this};
                        am_check_sign = new AttachManager<DangerSign>{check_sign, *this};
                    }
                }
                if(is_check() && is_mate())
                {
                    if(am_check_sign == nullptr)
                    {
                        DangerSign* check_sign = new DangerSign{find_king(step_chooser)->center(), *this};
                        am_check_sign = new AttachManager<DangerSign>{check_sign, *this};
                    }
                    show_checkmate_message();
                    step_chooser = none;
                }
                else if(is_stalemate())
                {
                    show_stalemate_message();
                    step_chooser = none;
                }
            }
        }

        selected->deactivate();

        // Clear the screen from visual representation of possible moves for the current figure
        if (all_possible_steps != nullptr)
        {
            delete all_possible_steps;
            all_possible_steps = nullptr;
        }
        selected = nullptr;
    }
    Fl::redraw();
}

bool Chessboard::decide()
{
    if (!selected->has_figure())
        return false;
    else if(step_chooser == step_color::none)
        return false;
    else if (step_chooser == step_color::white && selected->get_figure().is_black())
        return false;
    else if (step_chooser == step_color::black && selected->get_figure().is_white())
        return false;
    return true;
}

void Chessboard::reset_double_steps()
{
    for(int i = 0; i < cells.size(); i++)
        if(cells[i].has_figure() && cells[i].get_figure().is_pawn())
            cells[i].get_figure().reset_double_step();
}

bool Chessboard::out_of_range(Coordinate pos)
{
    if ((int(pos.x) < a_ascii) || (int(pos.x) > a_ascii + 7))
        return true;
    else if ((pos.y < 1) || (pos.y > 8))
        return true;
    return false;
}

Sub_Vector_ref Chessboard::operator[](char c)
{
    if(c < 'a' || c > 'h')
        Graph_lib::error("Out of range: horisontal");
    Graph_lib::Vector_ref<Cell> tempv;
    for(int i = 1; i <= N; i++)
        tempv.push_back(at(c, i));
    Sub_Vector_ref subv{tempv};
    return subv;
}

bool Chessboard::is_check()
{
    Cell* king_ptr = find_king(step_chooser);

    for(int i = a_ascii; i < a_ascii + N; i++)
    {
        for(int j = 1; j <= N; j++)
        {
            if(at(i,j).has_figure() &&
               ((step_chooser == white && at(i,j).get_figure().is_black()) ||
                (step_chooser == black && at(i,j).get_figure().is_white())))
            {
                if(at(i,j).get_figure().can_take_king(*this,*king_ptr))
                    return true;
            }
        }
    }
    return false;
}

bool Chessboard::is_mate()
{

    bool pawn_flag = false;

    bool first_step_reserved;
    bool double_step_reserved;
    int steps_till_reset_reserved;

    bool king_flag = false;
    bool rook_flag = false;

    bool can_do_castling_reserved;

    for(int i = a_ascii; i < a_ascii + N; i++)
    {
        for(int j = 1; j <= N; j++)
        {
            if(at(i,j).has_figure() &&
               ((step_chooser == white && at(i,j).get_figure().is_white()) ||
                (step_chooser == black && at(i,j).get_figure().is_black())))
            {
                for(int i_ = a_ascii; i_ < a_ascii + N; i_++)
                {
                    for(int j_ = 1; j_ <= N; j_++)
                    {
                        if(at(i,j).get_figure().is_pawn())
                        {
                            pawn_flag = true;

                            Pawn* pawn_fr = dynamic_cast<Pawn*>(&(at(i,j).get_figure()));

                            first_step_reserved = pawn_fr->first_step;
                            double_step_reserved = pawn_fr->double_step;
                            steps_till_reset_reserved = pawn_fr->steps_till_reset;
                        }
                        if(at(i,j).get_figure().is_king())
                        {
                            king_flag = true;

                            King* king_fr = dynamic_cast<King*>(&(at(i,j).get_figure()));

                            can_do_castling_reserved = king_fr->can_do_castling;
                        }
                        if(at(i,j).get_figure().is_rook())
                        {
                            rook_flag = true;

                            Rook* rook_fr = dynamic_cast<Rook*>(&(at(i,j).get_figure()));

                            can_do_castling_reserved = rook_fr->can_do_castling;
                        }

                        bool cond = !(i == i_ && j == j_) && at(i,j).get_figure().correct_step(at(i,j), at(i_,j_), *this, true) > 0;

                        if(pawn_flag)
                        {
                            Pawn* pawn_fr = dynamic_cast<Pawn*>(&(at(i,j).get_figure()));

                            pawn_fr->first_step = first_step_reserved;
                            pawn_fr->double_step = double_step_reserved;
                            pawn_fr->steps_till_reset = steps_till_reset_reserved;
                        }
                        else if(king_flag)
                        {
                            King* king_fr = dynamic_cast<King*>(&(at(i,j).get_figure()));

                            king_fr->can_do_castling = can_do_castling_reserved;
                        }
                        else if(rook_flag)
                        {
                            Rook* rook_fr = dynamic_cast<Rook*>(&(at(i,j).get_figure()));

                            rook_fr->can_do_castling = can_do_castling_reserved;
                        }

                        pawn_flag = false;
                        king_flag = false;
                        rook_flag = false;

                        if(cond)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Chessboard::is_stalemate()
{
    bool big_statement = true;

    for(int i = 0; i < pawns.size(); i++)
        if(pawns[i].has_cell())
            big_statement = false;
    for(int i = 0; i < rooks.size(); i++)
        if(rooks[i].has_cell())
            big_statement = false;
    for(int i = 0; i < knights.size(); i++)
        if(knights[i].has_cell())
            big_statement = false;
    for(int i = 0; i < bishops.size(); i++)
        if(bishops[i].has_cell())
            big_statement = false;
    for(int i = 0; i < queens.size(); i++)
        if(queens[i].has_cell())
            big_statement = false;
    for(int i = 0; i < kings.size(); i++)
        if(!(kings[i].has_cell()))
            throw std::runtime_error("No king!");

    return ((!is_check() && is_mate()) || big_statement);
}

void Chessboard::show_checkmate_message()
{
    std::string who_won = ((step_chooser == white) ? "BLACKS WON" : "WHITES WON");

    Text* txt1 = new Text{Point{DFTBOF + 4*c_size - int(3*standard_font_size), DFTBOF + 4*c_size - standard_font_size},"CHECKMATE!"};
    Text* txt2 = new Text{Point{DFTBOF + 4*c_size - int(3*standard_font_size), DFTBOF + 4*c_size}, who_won};
    texts.push_back(txt1);
    texts[texts.size()-1].set_font_size(standard_font_size);
    this->attach(texts[texts.size()-1]);
    texts.push_back(txt2);
    texts[texts.size()-1].set_font_size(standard_font_size);
    this->attach(texts[texts.size()-1]);
}

void Chessboard::show_stalemate_message()
{
    Text* txt = new Text{Point{DFTBOF + 4*c_size - int(5.5*standard_font_size), DFTBOF + 4*c_size}, "IT'S A STALEMATE!"};
    texts.push_back(txt);
    texts[texts.size()-1].set_font_size(standard_font_size);
    this->attach(texts[texts.size()-1]);
}

Cell* Chessboard::find_king(step_color color)
{
    Cell* king_ptr = nullptr;

    for(int i = a_ascii; i < a_ascii + N; i++)
    {
        for(int j = 1; j <= N; j++)
        {
            if(at(i,j).has_figure() && at(i,j).get_figure().is_king() &&
               ((color == white) == at(i,j).get_figure().is_white()))
            {
                king_ptr = &(at(i,j));
            }
        }
    }
    if(king_ptr == nullptr)
        throw std::runtime_error("Chessboard::find_king(step_color color) : No king!");
    return king_ptr;
}
