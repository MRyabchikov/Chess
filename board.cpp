#include "board.h"
#include <iostream>

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
        all_possible_steps = nullptr;
        selected = &c;
        c.activate();  // подсвечивает
        if(selected->has_figure())
            all_possible_steps = c.get_figure().show_possible_steps(c.location(), *this);
        // std::cout << "2\n";
    }
    else
    {
        // std::cout << "3\n";
        if (selected->has_figure())
        {
            if(selected->get_figure().correct_step(*selected, c))
            {
            //if()
            // move_figure
            Cell& c1 = *selected;
            if(c.has_figure())
            {

                detach(c.detach_figure());
                c.attach_figure(c1.detach_figure());
            }
            else
                c.attach_figure(c1.detach_figure());
            // std::cout << "4\n";
            }
        }

        selected->deactivate();
        if(all_possible_steps != nullptr)
        {
            delete all_possible_steps;
            all_possible_steps = nullptr;
        }
        /*
        if (selected == &c)
        {
            selected = nullptr;
        }
        else
        {
            selected = &c;
            c.activate();
        }
        */
       selected = nullptr;
    }
    Fl::redraw();
}
