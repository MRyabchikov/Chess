#include "figure.h"
#include "board.h"
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <vector>

using Graph_lib::Circle;

bool King_is_under_attack (Chessboard& chess, bool is_white)
{
    Cell* king_ptr = nullptr;
    for (int i = 0; i < chess.N; i++)
        for (int j = 1; j <= chess.N; j++)
            if (chess.at(i + a_ascii, j).has_figure() && chess.at(i + a_ascii, j).get_figure().is_king() &&
                chess.at(i + a_ascii, j).get_figure().is_white() == is_white)
                king_ptr = &(chess.at(i + a_ascii, j));
    if (king_ptr == nullptr)
        throw std::runtime_error("No king!");
    for (int i = 0; i < chess.N; i++)
        for (int j = 1; j <= chess.N; j++)
            if (chess.at(i + a_ascii, j).has_figure() && chess.at(i + a_ascii, j).get_figure().is_white() != is_white &&
                chess.at(i + a_ascii, j).get_figure().can_take_king(chess, *king_ptr))
                return true;
    return false;
}

void Figure::attach(const Cell& c)
{
    move(c.center().x - point(0).x - 40, c.center().y - point(0).y - 45);
    cell = &c;
}

bool Figure::change_pos_decider(Cell& c)
{
    if (c.has_figure())
    {
        if (is_white())
        {
            if (c.get_figure().is_white())
                return false;
        }
        else if (is_black())
        {
            if (c.get_figure().is_black())
                return false;
        }
    }
    return true;
}

int Pawn::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int decider;  // Decides whether to move upwards or downwards
                  // depending on the color of a figure.
                  // Решает, двигаться ли вверх или вниз, в зависимости от цвета фигуры

    // Decides whether to take the figure or not
    bool take_decider;  // If true, it assumes that it's whites' turn
                        // else - blacks' turn.
    int returning_value = 0;

    if (is_white())
        decider = 1;  // For some reason ternary operator didn't work so
    else              // I had to set 'decider' value the old-fashioned way
        decider = -1;
    if (decider == 1)
        take_decider = true;
    else
        take_decider = false;

    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if (chess.at(char(x1 - 1), y1).has_figure() && chess.at(char(x1 - 1), y1).get_figure().is_pawn() &&
        chess.at(char(x1), y1).get_figure().is_black() != chess.at(char(x1 - 1), y1).get_figure().is_black() &&
        chess.at(char(x1 - 1), y1).get_figure().double_step0())
    {
        returning_value = 2;
    }
    else if (chess.at(char(x1 + 1), y1).has_figure() && chess.at(char(x1 + 1), y1).get_figure().is_pawn() &&
             chess.at(char(x1), y1).get_figure().is_black() != chess.at(char(x1 + 1), y1).get_figure().is_black() &&
             chess.at(char(x1 + 1), y1).get_figure().double_step0())
    {
        return 3;
    }
    else if (first_step)
    {
        if (x1 == x2 && ((y2 - y1 == 1 * decider) || (y2 - y1 == 2 * decider)) && !c2.has_figure())
        {
            if (y2 - y1 == 2 * decider)
                if (chess.at(char(x1), y1 + 1 * decider).has_figure())
                    return 0;
            if (y2 - y1 == 2 * decider && double_step == false)
                double_step = 1;
            first_step = false;
            returning_value = 1;
        }

        else if ((x1 == x2 + 1 || x1 == x2 - 1) && (y2 - y1 == 1 * decider) && c2.has_figure() &&
                 c2.get_figure().is_black() == take_decider)
        {
            first_step = false;
            double_step = false;
            returning_value = 1;
        }
        else
            return 0;
    }
    else
    {
        if (x1 == x2 && (y2 - y1 == 1 * decider) && !c2.has_figure())
        {
            double_step = 0;
            returning_value = 1;
        }
        else if ((x1 == x2 + 1 || x1 == x2 - 1) && (y2 - y1 == 1 * decider) && c2.has_figure() &&
                 c2.get_figure().is_black() == take_decider)
        {
            double_step = 0;
            returning_value = 1;
        }
        else
            return 0;
    }
    if (ensure_king_is_safe == false)
        return returning_value;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            std::cout << c2.location().x << " " << c2.location().y << c2.get_figure().is_white() << " " << std::endl;
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return 0;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return returning_value;
        }
    }
}

VisualSteps* Pawn::show_possible_steps(Coordinate position, Chessboard& chess)  // moves on the board
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    int decider;

    char x = position.x;
    int y = position.y;

    if (is_white())
        decider = 1;
    else
        decider = -1;

    if (position.y == int(4.5 + 3.5 * decider))  // If pawn has reached the end of the board
        return steps_representation;             // It can't move anymore (for now)

    if (position.y == int(4.5 + 3.5 * decider))  // If pawn has reached the end of the board
        return steps_representation;             // It can't move anymore (for now)

    bool first_step_reserved = first_step;  // Костыль

    for (int i = 1; i <= (first_step ? 2 : 1); i++)
    {
        if (correct_step(chess.at(x, y), chess.at(x, y + i * decider), chess))
        {

            first_step = first_step_reserved;  // Костыль

            Circle* tempc = new Circle{chess.at(x, y + i * decider).center(), c_size / 4};
            tempc->set_color(chess_yellow);
            tempc->set_fill_color(chess_yellow);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            // delete tempc;
        }
    }
    for (int i = -1; i <= 1; i += 2)
    {
        if (!(int(x) + i < a_ascii || int(x) + i > 7 + a_ascii))
        {
            int a = correct_step(chess.at(x, y), chess.at(char(int(x) + i), y + decider * 1), chess);
            if (a)
            {
                first_step = first_step_reserved;  // Костыль

                Frame* tempf;
                if (a == 1)
                    tempf = new Frame{chess.at(char(int(x) + i), y + decider).center(), chess};
                if (a == 2)
                    tempf = new Frame{chess.at(char(int(x) - 1), y + decider).center(), chess};
                if (a == 3)
                    tempf = new Frame{chess.at(char(int(x) + 1), y + decider).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                // delete tempf;
            }
        }
    }
    return steps_representation;
}

int Rook::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if ((x1 != x2) && (y1 != y2))
        return false;
    if ((x1 == x2) && (y1 == y2))
        return false;

    int d;  // d - decider, used to reduce copy + paste

    if ((x1 != x2) && (y1 == y2))
    {
        d = (x2 > x1) ? 1 : -1;

        for (int i = x1 + d; i != x2; i += d)
            if (chess.at(char(i), y1).has_figure())
                return false;
        if (change_pos_decider(c2) == false)
            return false;
    }
    if ((x1 == x2) && (y1 != y2))
    {
        d = (y2 > y1) ? 1 : -1;

        for (int i = y1 + d; i != y2; i += d)
            if (chess.at(char(x1), i).has_figure())
                return false;
        if (change_pos_decider(c2) == false)
            return false;
    }
    if (ensure_king_is_safe == false)
        return true;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return false;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return true;
        }
    }
}

VisualSteps* Rook::show_possible_steps(Coordinate position, Chessboard& chess)
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    horisontal_possible_steps(position, chess, steps_representation);
    vertical_possible_steps(position, chess, steps_representation);

    return steps_representation;
}

void Rook::horisontal_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation)
{
    // d is made to reduce copy + paste
    for (int d = -1; d <= 1; d += 2)
    {
        for (int i = int(position.x) + d; i != a_ascii - 1 + (4.5 + 4.5 * d); i += d)
        {
            if (correct_step(chess.at(position.x, position.y), chess.at(char(i), position.y), chess))
            {
                if (chess.at(char(i), position.y).has_figure())
                {
                    Frame* tempf = new Frame{chess.at(char(i), position.y).center(), chess};
                    steps_representation->possible_takes.push_back(tempf);
                    chess.attach(*steps_representation->possible_takes.back());
                    // delete tempf;
                    break;
                }
                else
                {
                    Circle* tempc = new Circle{chess.at(char(i), position.y).center(), c_size / 4};
                    tempc->set_color(chess_yellow);
                    tempc->set_fill_color(chess_yellow);
                    steps_representation->possible_steps.push_back(tempc);
                    chess.attach(*steps_representation->possible_steps.back());
                    // delete tempc;
                }
            }
        }
    }
}

void Rook::vertical_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation)
{
    // d is made to reduce copy + paste
    for (int d = -1; d <= 1; d += 2)
    {
        for (int i = position.y + d; i != int(4.5 + 4.5 * d); i += d)
        {
            if (correct_step(chess.at(position.x, position.y), chess.at(position.x, i), chess))
            {
                if (chess.at(position.x, i).has_figure())
                {
                    Frame* tempf = new Frame{chess.at(position.x, i).center(), chess};
                    steps_representation->possible_takes.push_back(tempf);
                    chess.attach(*steps_representation->possible_takes.back());
                    // delete tempf;
                    break;
                }
                else
                {
                    Circle* tempc = new Circle{chess.at(position.x, i).center(), c_size / 4};
                    tempc->set_color(chess_yellow);
                    tempc->set_fill_color(chess_yellow);
                    steps_representation->possible_steps.push_back(tempc);
                    chess.attach(*steps_representation->possible_steps.back());
                    // delete tempc;
                }
            }
        }
    }
}

int Knight::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if (!((std::abs(x2 - x1) == 1 && std::abs(y2 - y1) == 2) || (std::abs(y2 - y1) == 1 && std::abs(x2 - x1) == 2)))
        return false;

    if ((std::abs(x2 - x1) == 1) && (std::abs(y2 - y1) == 2))
    {
        if (c2.has_figure())
        {
            if (change_pos_decider(c2) == false)
                return false;
        }
    }
    else if ((std::abs(x2 - x1) == 2) && (std::abs(y2 - y1) == 1))
    {
        if (c2.has_figure())
        {
            if (change_pos_decider(c2) == false)
                return false;
        }
    }
    if (ensure_king_is_safe == false)
        return true;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return false;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return true;
        }
    }
}

VisualSteps* Knight::show_possible_steps(Coordinate position, Chessboard& chess)
{

    VisualSteps* steps_representation = new VisualSteps{chess};

    int x = int(position.x);
    int y = position.y;

    for (int i = 0; i < chess.N; i++)
        for (int j = 1; j <= chess.N; j++)
        {
            Coordinate pos = Coordinate(i + a_ascii, j);
            if (!chess.out_of_range(pos))
            {
                if (correct_step(chess.at(char(x), y), chess.at(pos.x, pos.y), chess))
                {
                    if (chess.at(pos.x, pos.y).has_figure())
                    {
                        Frame* tempf = new Frame{chess.at(pos.x, pos.y).center(), chess};
                        steps_representation->possible_takes.push_back(tempf);
                        chess.attach(*steps_representation->possible_takes.back());
                        // delete tempf;
                    }
                    else
                    {
                        Circle* tempc = new Circle{chess.at(pos.x, pos.y).center(), c_size / 4};
                        tempc->set_color(chess_yellow);
                        tempc->set_fill_color(chess_yellow);
                        steps_representation->possible_steps.push_back(tempc);
                        chess.attach(*steps_representation->possible_steps.back());
                        // delete tempc;
                    }
                }
            }
        }
    return steps_representation;
}

int Bishop::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if (std::abs(x2 - x1) != std::abs(y2 - y1))
        return false;
    else if (x2 == x1 && y2 == y1)
        return false;

    int d1, d2;  // d1, d2 - decider1, decider2, created to reduce copy + paste

    d1 = (x2 > x1) ? 1 : -1;
    d2 = (y2 > y1) ? 1 : -1;

    int x = x1 + d1;
    int y = y1 + d2;

    while (!chess.out_of_range(Coordinate{char(x), y}) && x != x2 && y != y2)
    {
        if (chess.at(char(x), y).has_figure())
            return false;
        x += d1;
        y += d2;
    }
    if (change_pos_decider(c2) == false)
        return false;
    if (ensure_king_is_safe == false)
        return true;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return false;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return true;
        }
    }
}

VisualSteps* Bishop::show_possible_steps(Coordinate position, Chessboard& chess)
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    int x0 = int(position.x);
    int y0 = position.y;

    int x, y;

    // d1, d2 - decider1, decider2 - those are used to reduce copy + paste
    for (int d1 = -1; d1 <= 1; d1 += 2)
    {
        for (int d2 = -1; d2 <= 1; d2 += 2)
        {
            x = int(position.x) + d1;
            y = position.y + d2;

            show_possible_steps_HF(x, y, x0, y0, d1, d2, steps_representation, chess);
        }
    }

    return steps_representation;
}

void Bishop::show_possible_steps_HF(int x, int y, int x0, int y0, int d1, int d2, VisualSteps*& steps_representation,
                                    Chessboard& chess)
{
    while (!chess.out_of_range(Coordinate{char(x), y}))
    {
        if (correct_step(chess.at(char(x0), y0), chess.at(char(x), y), chess))
        {
            if (chess.at(char(x), y).has_figure())
            {
                Frame* tempf = new Frame{chess.at(char(x), y).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                // delete tempf;
                break;
            }
            else
            {
                Circle* tempc = new Circle{chess.at(char(x), y).center(), c_size / 4};
                tempc->set_color(chess_yellow);
                tempc->set_fill_color(chess_yellow);
                steps_representation->possible_steps.push_back(tempc);
                chess.attach(*steps_representation->possible_steps.back());
            }
        }
        x += d1;
        y += d2;
    }
}

int Queen::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if (x1 == x2 && y1 == y2)
        return false;

    bool cond1 = (std::abs(x2 - x1) == std::abs(y2 - y1));
    bool cond2 = (x2 == x1 && y2 != y1);
    bool cond3 = (x2 != x1 && y2 == y1);

    if (!cond1 && !cond2 && !cond3)
        return false;

    if (((x1 != x2) && (y1 == y2)) || ((x1 == x2) && (y1 != y2)))
    {
        int d;  // d - decider, used to reduce copy + paste

        if ((x1 != x2) && (y1 == y2))
        {
            d = (x2 > x1) ? 1 : -1;

            for (int i = x1 + d; i != x2; i += d)
                if (chess.at(char(i), y1).has_figure())
                    return false;
            if (change_pos_decider(c2) == false)
                return false;
        }
        if ((x1 == x2) && (y1 != y2))
        {
            d = (y2 > y1) ? 1 : -1;

            for (int i = y1 + d; i != y2; i += d)
                if (chess.at(char(x1), i).has_figure())
                    return false;
            if (change_pos_decider(c2) == false)
                return false;
        }
    }
    else if (x1 != x2 && y1 != y2)
    {
        int d1, d2;  // d1, d2 - decider1, decider2, created to reduce copy + paste

        d1 = (x2 > x1) ? 1 : -1;
        d2 = (y2 > y1) ? 1 : -1;

        int x = x1 + d1;
        int y = y1 + d2;

        while (!chess.out_of_range(Coordinate{char(x), y}) && x != x2 && y != y2)
        {
            if (chess.at(char(x), y).has_figure())
                return false;
            x += d1;
            y += d2;
        }
        if (change_pos_decider(c2) == false)
            return false;
    }
    if (ensure_king_is_safe == false)
        return true;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return false;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return true;
        }
    }
}

VisualSteps* Queen::show_possible_steps(Coordinate position, Chessboard& chess)
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    int x0 = int(position.x);
    int y0 = position.y;

    int x, y;

    horisontal_possible_steps(position, chess, steps_representation);
    vertical_possible_steps(position, chess, steps_representation);
    diagnal_possible_steps(x, y, x0, y0, position, chess, steps_representation);

    return steps_representation;
}

void Queen::horisontal_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation)
{
    // d is made to reduce copy + paste
    for (int d = -1; d <= 1; d += 2)
    {
        for (int i = int(position.x) + d; i != a_ascii - 1 + (4.5 + 4.5 * d); i += d)
        {
            if (correct_step(chess.at(position.x, position.y), chess.at(char(i), position.y), chess))
            {
                if (chess.at(char(i), position.y).has_figure())
                {
                    Frame* tempf = new Frame{chess.at(char(i), position.y).center(), chess};
                    steps_representation->possible_takes.push_back(tempf);
                    chess.attach(*steps_representation->possible_takes.back());
                    // delete tempf;
                    break;
                }
                else
                {
                    Circle* tempc = new Circle{chess.at(char(i), position.y).center(), c_size / 4};
                    tempc->set_color(chess_yellow);
                    tempc->set_fill_color(chess_yellow);
                    steps_representation->possible_steps.push_back(tempc);
                    chess.attach(*steps_representation->possible_steps.back());
                    // delete tempc;
                }
            }
        }
    }
}

void Queen::vertical_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps*& steps_representation)
{
    // d is made to reduce copy + paste
    for (int d = -1; d <= 1; d += 2)
    {
        for (int i = position.y + d; i != int(4.5 + 4.5 * d); i += d)
        {
            if (correct_step(chess.at(position.x, position.y), chess.at(position.x, i), chess))
            {
                if (chess.at(position.x, i).has_figure())
                {
                    Frame* tempf = new Frame{chess.at(position.x, i).center(), chess};
                    steps_representation->possible_takes.push_back(tempf);
                    chess.attach(*steps_representation->possible_takes.back());
                    // delete tempf;
                    break;
                }
                else
                {
                    Circle* tempc = new Circle{chess.at(position.x, i).center(), c_size / 4};
                    tempc->set_color(chess_yellow);
                    tempc->set_fill_color(chess_yellow);
                    steps_representation->possible_steps.push_back(tempc);
                    chess.attach(*steps_representation->possible_steps.back());
                    // delete tempc;
                }
            }
        }
    }
}

void Queen::diagnal_possible_steps(int x, int y, int x0, int y0, Coordinate& position, Chessboard& chess,
                                   VisualSteps*& steps_representation)
{
    // d1, d2 - decider1, decider2 - those are used to reduce copy + paste
    for (int d1 = -1; d1 <= 1; d1 += 2)
    {
        for (int d2 = -1; d2 <= 1; d2 += 2)
        {
            x = int(position.x) + d1;
            y = position.y + d2;

            show_possible_steps_HF(x, y, x0, y0, d1, d2, steps_representation, chess);
        }
    }
}

void Queen::show_possible_steps_HF(int x, int y, int x0, int y0, int d1, int d2, VisualSteps*& steps_representation,
                                   Chessboard& chess)
{
    while (!chess.out_of_range(Coordinate{char(x), y}))
    {
        if (correct_step(chess.at(char(x0), y0), chess.at(char(x), y), chess))
        {
            if (chess.at(char(x), y).has_figure())
            {
                Frame* tempf = new Frame{chess.at(char(x), y).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                // delete tempf;
                break;
            }
            else
            {
                Circle* tempc = new Circle{chess.at(char(x), y).center(), c_size / 4};
                tempc->set_color(chess_yellow);
                tempc->set_fill_color(chess_yellow);
                steps_representation->possible_steps.push_back(tempc);
                chess.attach(*steps_representation->possible_steps.back());
            }
        }
        x += d1;
        y += d2;
    }
}

int King::correct_step(Cell& c1, Cell& c2, Chessboard& chess, bool ensure_king_is_safe)
{
    int x1 = int(c1.location().x), x2 = int(c2.location().x);
    int y1 = c1.location().y, y2 = c2.location().y;
    if (!(abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1) || (x2 == x1 && y1 == y2))
        return false;
    if (change_pos_decider(c2) == false)
        return false;
    if (ensure_king_is_safe == false)
        return true;
    else
    {
        bool has_deleted_figure = false;
        Figure* tmp = nullptr;
        if (c2.has_figure())
        {
            tmp = &(c2.get_figure());
            chess.detach(c2.detach_figure());
            has_deleted_figure = true;
        }
        c2.attach_figure(c1.detach_figure());
        if (King_is_under_attack(chess, c2.get_figure().is_white()))
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return false;
        }
        else
        {
            c1.attach_figure(c2.detach_figure());
            if (has_deleted_figure)
            {
                c2.attach_figure(*tmp);
                chess.attach(*tmp);
            }
            return true;
        }
    }
}

VisualSteps* King::show_possible_steps(Coordinate position, Chessboard& chess)
{
    // I know it's less effective, but easier to read, maybe should be rewritten
    VisualSteps* steps_representation = new VisualSteps{chess};
    for (int i = 0; i < chess.N; i++)
        for (int j = 1; j <= chess.N; j++)
        {
            if (correct_step(chess.at(position.x, position.y), chess.at(i + a_ascii, j), chess))
            {
                if (chess.at(i + a_ascii, j).has_figure())
                {
                    Frame* tempf = new Frame{chess.at(i + a_ascii, j).center(), chess};
                    steps_representation->possible_takes.push_back(tempf);
                    chess.attach(*steps_representation->possible_takes.back());
                }
                else
                {
                    Circle* tempc = new Circle{chess.at(i + a_ascii, j).center(), c_size / 4};
                    tempc->set_color(chess_yellow);
                    tempc->set_fill_color(chess_yellow);
                    steps_representation->possible_steps.push_back(tempc);
                    chess.attach(*steps_representation->possible_steps.back());
                }
            }
        }
    return steps_representation;
}