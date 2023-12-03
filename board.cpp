#include "board.h"
#include "cell.h"
#include <iostream>


Cell& Sub_Vector_ref::operator[](int i)
{
    if(i < 1 || i > 8)
        Graph_lib::error("Out of range");
    return v[i-1];
}

MyWindow::MyWindow(Point xy, int w, int h, const std::string& title)
    : Simple_window{xy, w, h, title}, quit_button{Point{x_max() - 70, 20}, 70, 20, "Quit", cb_quit}
{
    attach(quit_button);
}

void MyWindow::cb_quit(Address, Address widget)
{  // void*
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

void Chessboard::clicked(Cell& c)
{  // для шашек. Для всего остального удалить
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
            if (a)
            {
                // if()
                //  move_figure
                Cell& c1 = *selected;
                int x = c.location().x, y = c.location().y;

                int b;
                if (step_chooser == black)
                    b = 1;
                else
                    b = -1;
                if (c.has_figure())
                {
                    // taking the figure from the opponent
                    detach(c.detach_figure());            // убираем фигуру врага
                    c.attach_figure(c1.detach_figure());  // переносим свою
                }
                else if (1 <= y + b && y + b < 7 && (*this)[x][y + b].has_figure() && (a == 2 || a == 3))
                {
                    detach((*this)[x][y + b].detach_figure());  // *this = chess
                    (*this)[x][y].attach_figure(c1.detach_figure());
                }
                else
                {
                    c.attach_figure(c1.detach_figure());
                }

                step_swap();
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
    if (step_chooser == step_color::white && selected->get_figure().is_black())
        return false;
    else if (step_chooser == step_color::black && selected->get_figure().is_white())
        return false;
    return true;
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
        Graph_lib::error("Out of range");
    Graph_lib::Vector_ref<Cell> tempv;
    for(int i = 1; i <= N; i++)
        tempv.push_back(at(c, i));
    Sub_Vector_ref subv{tempv};
    return subv;
}
