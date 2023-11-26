#include "figure.h"
#include "board.h"
#include <algorithm>
#include <iostream>

using Graph_lib::Circle;

void Figure::attach(const Cell& c)
{
    move(c.center().x - point(0).x - 40, c.center().y - point(0).y - 45);
    cell = &c;
}

bool Figure::change_pos_decider(Cell& c)
{
    if(c.has_figure())
    {
        if(is_white())
        {
            if(c.get_figure().is_white())
                return false;
        }
        else if(is_black())
        {
            if(c.get_figure().is_black())
                return false;
        }
    }
    return true;
}

bool Pawn::correct_step(Cell& c1, Cell& c2, Chessboard& chess)
{
    int decider;       //Decides whether to move upwards or downwards
                       //depending on the color of a figure

                       //Decides whether to take the figure or not
    bool take_decider; //If true, it assumes that it's whites' turn
                       //else - blacks' turn

    if(is_white())    
        decider = 1;   //For some reason ternary operator didn't work so
    else               //I had to set 'decider' value the old-fashioned way
        decider = -1;

    if(decider == 1)
        take_decider = true;
    else
        take_decider = false;
    
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if(first_step)
    {
        if(x1 == x2 && ((y2 - y1 == 1 * decider) || (y2 - y1 == 2 * decider)) && !c2.has_figure())
        {
            if(y2 - y1 == 2 * decider)
                if(chess.at(char(x1), y1 + 1 * decider).has_figure())
                    return false;
            first_step = false;
            return true;
        }
        else if((x1 == x2 + 1 || x1 == x2 - 1) && (y2 - y1 == 1 * decider) && c2.has_figure() &&
                c2.get_figure().is_black() == take_decider
                )
        {
            first_step = false;
            return true;
        }
        else
            return false;
    }
    else
    {
        if(x1 == x2 && (y2 - y1 == 1 * decider) && !c2.has_figure())
            return true;
        else if((x1 == x2 + 1 || x1 == x2 - 1) && (y2 - y1 == 1 * decider) && c2.has_figure() && 
                c2.get_figure().is_black() == take_decider
                )
            return true;
        else
            return false;
    }
}

VisualSteps* Pawn::show_possible_steps(Coordinate position, Chessboard& chess) //Shows all possible
                                                                               //moves on the board
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    int decider;

    int x = position.x;
    int y = position.y;

    if(is_white())    
        decider = 1;   
    else               
        decider = -1;

    if(position.y == int(4.5 + 3.5*decider))  //If pawn has reached the end of the board
        return steps_representation;          //It can't move anymore (for now)

    bool first_step_reserved = first_step; //Костыль

    for(int i = 1; i <= (first_step ? 2 : 1); i++)
    {
        if(correct_step(chess.at(x,y),chess.at(x,y + i*decider),chess))
        {
            first_step = first_step_reserved; //Костыль

            Circle* tempc = new Circle{chess.at(x,y + i*decider).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }
    for(int i = -1; i <= 1; i += 2)
    {
        if(!(int(x) + i < a_ascii || int(x) + i > 7 + a_ascii))
        {
            if(correct_step(chess.at(x, y),chess.at(char(int(x) + i), y + decider*1), chess))
            {

                first_step = first_step_reserved; //Костыль

                Frame* tempf = new Frame{chess.at(char(int(x) + i),y + decider).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
        }
    }
    return steps_representation;
}

bool Rook::correct_step(Cell& c1, Cell& c2, Chessboard& chess)
{
    int x1 = int(c1.location().x);
    int y1 = c1.location().y;
    int x2 = int(c2.location().x);
    int y2 = c2.location().y;

    if((x1 != x2) && (y1 != y2))
        return false;
    if((x1 == x2) && (y1 == y2))
        return false;
    if((x1 != x2) && (y1 == y2))
    {
        if(x2 > x1)
        {
            for(int i = x1 + 1; i < x2; i++)
                if(chess.at(char(i), y1).has_figure())
                    return false;
            if(change_pos_decider(c2) == false)
                return false;
        }
        else if(x2 > x1)
        {
            for(int i = x1 - 1; i > x2; i--)
                if(chess.at(char(i), y1).has_figure())
                    return false;
            if(change_pos_decider(c2) == false)
                return false;
        }
    }
    if((x1 == x2) && (y1 != y2))
    {
        if(y2 > y1)
        {
            for(int i = y1 + 1; i < y2; i++)
                if(chess.at(char(x1), i).has_figure())
                    return false;
            if(change_pos_decider(c2) == false)
                return false;
        }
        if(y2 < y1)
        {
            for(int i = y1 - 1; i > y2; i--)
                if(chess.at(char(x1), i).has_figure())
                    return false;
            if(change_pos_decider(c2) == false)
                return false;
        }
    }
    return true;
}

VisualSteps* Rook::show_possible_steps(Coordinate position, Chessboard& chess)
{
    VisualSteps* steps_representation = new VisualSteps{chess};

    horisontal_possible_steps(position, chess, steps_representation);
    vertical_possible_steps(position, chess, steps_representation);

    return steps_representation;
}

void Rook::horisontal_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation)
{
    for(int i = int(position.x)+1; i < a_ascii + 7 + 1; i++)
    {
        if(chess.at(char(i),position.y).has_figure())
        {
            if(correct_step(chess.at(char(int(position.x)),position.y), chess.at(char(i),position.y), chess))
            {
                Frame* tempf = new Frame{chess.at(char(i),position.y).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
            break;
        }

        if(correct_step(chess.at(position.x,position.y), chess.at(char(i),position.y), chess))
        {
            Circle* tempc = new Circle{chess.at(char(i),position.y).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }
    for(int i = int(position.x)-1; i > a_ascii - 1; i--)
    {
        if(chess.at(char(i),position.y).has_figure())
        {
            if(correct_step(chess.at(position.x,position.y), chess.at(char(i),position.y), chess))
            {
                Frame* tempf = new Frame{chess.at(char(i),position.y).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
            break;
        }

        if(correct_step(chess.at(position.x,position.y), chess.at(char(i),position.y), chess))
        {
            Circle* tempc = new Circle{chess.at(char(i),position.y).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }
}

void Rook::vertical_possible_steps(Coordinate& position, Chessboard& chess, VisualSteps* & steps_representation)
{
    for(int i = position.y+1; i < 8 + 1; i++)
    {
        if(chess.at(position.x,i).has_figure())
        {
            if(correct_step(chess.at(position.x,position.y), chess.at(position.x,i), chess))
            {
                Frame* tempf = new Frame{chess.at(position.x,i).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
            break;
        }

        if(correct_step(chess.at(position.x,position.y), chess.at(position.x, i), chess))
        {
            Circle* tempc = new Circle{chess.at(position.x,i).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }

    for(int i = position.y-1; i > 0; i--)
    {
        if(chess.at(position.x,i).has_figure())
        {
            if(correct_step(chess.at(position.x,position.y), chess.at(position.x,i), chess))
            {
                Frame* tempf = new Frame{chess.at(position.x,i).center(), chess};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
            break;
        }

        if(correct_step(chess.at(position.x,position.y), chess.at(position.x, i), chess))
        {
            Circle* tempc = new Circle{chess.at(position.x,i).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }
}