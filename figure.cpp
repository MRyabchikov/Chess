#include "figure.h"
#include "cell.h"
#include "board.h"
#include <algorithm>
#include <iostream>

using Graph_lib::Circle;

void Figure::attach(const Cell& c)
{
    move(c.center().x - point(0).x - 40, c.center().y - point(0).y - 45);
    cell = &c;
}


bool Pawn::correct_step(Cell& c1, Cell& c2)
{
    int decider; //Decides whether to move upwards or downwards
                 //depending on the color of a figure

                       //Decides wheter to take the figure or not
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
    

    int x1 = c1.location().x;
    int y1 = c1.location().y;
    int x2 = c2.location().x;
    int y2 = c2.location().y;
    if(first_step)
    {
        if(x1 == x2 && ((y2 - y1 == 1 * decider) || (y2 - y1 == 2 * decider)) && !c2.has_figure())
        {
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

VisualSteps* Pawn::show_possible_steps(Coordinate position, Chessboard& chess)
{
    VisualSteps* steps_representation = new VisualSteps;

    int decider;
    if(is_white())    
        decider = 1;   
    else               
        decider = -1;

    std::cout << "***THIS IS A TEST***\n" <<
              (char)(position.x + a_ascii) << ' ' << position.y << '\n' <<
              (char)(position.x + a_ascii) << ' ' << position.y + 1*decider << '\n' <<
              (char)(position.x + a_ascii) << ' ' << position.y + 2*decider << '\n';
    std::cout << (char)(position.x + a_ascii - 1) << ' ' << position.y + 1*decider << '\n';
    std::cout << (char)(position.x + a_ascii + 1) << ' ' << position.y + 1*decider << '\n';
    std::cout << "***END OF THE TEST***\n";

    //Cell = chess.at(char(position.x + a_ascii), position.y)

    bool first_step_reserved = first_step; //Костыль

    for(int i = 1; i <= (first_step ? 2 : 1); i++)
    {
        if(correct_step(
                        chess.at((char)(position.x + a_ascii),position.y),
                        chess.at((char)(position.x + a_ascii),position.y + i*decider)
                        ))
        {
            first_step = first_step_reserved; //Костыль

            Circle* tempc = new Circle{chess.at((char)(position.x + a_ascii -1),position.y+i*decider).center(), c_size/4};
            tempc->set_color(Graph_lib::Color::green);
            tempc->set_fill_color(Graph_lib::Color::green);
            steps_representation->possible_steps.push_back(tempc);
            chess.attach(*steps_representation->possible_steps.back());
            //delete tempc;
        }
    }
    for(int i = -1; i <= 1; i += 2)
    {
        if(!(position.x - 1 + i < 0 || position.x + i - 1 > 7))
        {
            if(correct_step
            (chess.at((char)(position.x + a_ascii), position.y),
             chess.at((char)(position.x + a_ascii + i), position.y + decider*1))
              )
            {

                first_step = first_step_reserved; //Костыль

                Frame* tempf = new Frame{chess.at((char)(position.x + a_ascii+i-1),position.y + decider).center()};
                steps_representation->possible_takes.push_back(tempf);
                chess.attach(*steps_representation->possible_takes.back());
                //delete tempf;
            }
        }
    }
    return steps_representation;
}


Frame::Frame(Point center) : 
    Rectangle({center.x - c_size/2, center.y - c_size/2}, c_size, c_size)
    {
        Rectangle* h_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_width, rc_length};
        Rectangle* h_r2 = new Rectangle{{center.x + c_size/2 - rc_width, center.y - c_size/2}, rc_width, rc_length};
        Rectangle* h_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_length}, rc_width, rc_length};
        Rectangle* h_r4 = new Rectangle{{center.x + c_size/2 - rc_width, center.y + c_size/2 - rc_length}, rc_width, rc_length};

        h_r1->set_fill_color(Graph_lib::Color::green);
        h_r2->set_fill_color(Graph_lib::Color::green);
        h_r3->set_fill_color(Graph_lib::Color::green);
        h_r4->set_fill_color(Graph_lib::Color::green);

        h_r1->set_color(Graph_lib::Color::green);
        h_r2->set_color(Graph_lib::Color::green);
        h_r3->set_color(Graph_lib::Color::green);
        h_r4->set_color(Graph_lib::Color::green);

        horisontal_rectangles.push_back(h_r1);
        horisontal_rectangles.push_back(h_r2);
        horisontal_rectangles.push_back(h_r3);
        horisontal_rectangles.push_back(h_r4);

        /*There are probably ways to avoid Copy+Paste but there's 
          something with Graph_lib that doesn't allow it to be easy*/

        Rectangle* v_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_length, rc_width};
        Rectangle* v_r2 = new Rectangle{{center.x + c_size/2 - rc_length, center.y - c_size/2}, rc_length, rc_width};
        Rectangle* v_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_width}, rc_length, rc_width};
        Rectangle* v_r4 = new Rectangle{{center.x + c_size/2 - rc_length, center.y + c_size/2 - rc_width}, rc_length, rc_width};

        v_r1->set_fill_color(Graph_lib::Color::green);
        v_r2->set_fill_color(Graph_lib::Color::green);
        v_r3->set_fill_color(Graph_lib::Color::green);
        v_r4->set_fill_color(Graph_lib::Color::green);

        v_r1->set_color(Graph_lib::Color::green);
        v_r2->set_color(Graph_lib::Color::green);
        v_r3->set_color(Graph_lib::Color::green);
        v_r4->set_color(Graph_lib::Color::green);

        vertical_rectangles.push_back(v_r1);
        vertical_rectangles.push_back(v_r2);
        vertical_rectangles.push_back(v_r3);
        vertical_rectangles.push_back(v_r4);
    }

Frame::~Frame()
{
    for(int i = 0; i < (int)horisontal_rectangles.size(); i++)
    {
        delete horisontal_rectangles[i];
        delete vertical_rectangles[i];
    }
}

void Frame::draw_lines() const
{
    horisontal_rectangles[0]->draw_lines();
    horisontal_rectangles[1]->draw_lines();
    horisontal_rectangles[2]->draw_lines();
    horisontal_rectangles[3]->draw_lines();

    vertical_rectangles[0]->draw_lines();
    vertical_rectangles[1]->draw_lines();
    vertical_rectangles[2]->draw_lines();
    vertical_rectangles[3]->draw_lines();
}

VisualSteps::~VisualSteps()
{
    for(int i = 0; i < possible_steps.size(); i++)
        delete possible_steps[i];
    for(int i = 0; i < possible_takes.size(); i++)
        delete possible_takes[i];
}