#include "steps_representation.h"
#include "board.h"


DangerSign::DangerSign(Point center, Chessboard& chess_) : Circle{center, c_size/2}
{
    for(int i = center.x - c_size/4; i <= center.x + c_size/4; i += c_size/2)
    {
        for(int j = center.y - c_size/4; j <= center.y + c_size/4; j += c_size/2)
        {
            Circle* temporary_circle = new Circle{{i,j}, circle_radius};
            temporary_circle->set_fill_color(Graph_lib::Color::red);
            temporary_circle->set_color(Graph_lib::Color::red);
            circle_of_circles.push_back(temporary_circle);
        }
    }
    for(int i = center.x - c_size/4 - c_size/8; i <= center.x + c_size/4 + c_size/8; i += c_size/4 + c_size/8)
    {
        Circle* temporary_circle = new Circle{{i,center.y}, circle_radius};
        temporary_circle->set_fill_color(Graph_lib::Color::red);
        temporary_circle->set_color(Graph_lib::Color::red);
        circle_of_circles.push_back(temporary_circle);
    }
    for(int j = center.y - c_size/4 - c_size/8; j <= center.y + c_size/4 + c_size/8; j += c_size/2 + c_size/4)
    {
        Circle* temporary_circle = new Circle{{center.x,j}, circle_radius};
        temporary_circle->set_fill_color(Graph_lib::Color::red);
        temporary_circle->set_color(Graph_lib::Color::red);
        circle_of_circles.push_back(temporary_circle);
    }

    chess = &chess_;
}

//I am very unsure of this destructor
DangerSign::~DangerSign()
{
    for(int i = (int)circle_of_circles.size() - 1; i >= 0; i--)
    {
        //Graph_lib::Circle::~Circle();
        chess->detach(*circle_of_circles[i]);
        delete circle_of_circles[i];
        circle_of_circles.erase(circle_of_circles.begin()+i,circle_of_circles.begin()+i+1);
        chess->detach(*this);
    }
}

void DangerSign::draw_lines() const
{
    for(int i = 0; i < int(circle_of_circles.size()); i++)
        circle_of_circles[i]->draw_lines();
}

RedCross::RedCross(Point center, Chessboard& chess_) : Rectangle{center, c_size, c_size}
{
    int x = center.x;
    int y = center.y;

    rectangle_1 = new Closed_polyline{{x - c_size/2 + 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y + c_size/2 - 2*dist},
                                      {x + c_size/2 - 2*dist, y + c_size/2 - dist}, {x - c_size/2 + dist, y - c_size/2 + 2*dist}};
    rectangle_1->set_color(Graph_lib::Color::red);
    rectangle_1->set_fill_color(Graph_lib::Color::red);

    rectangle_2 = new Closed_polyline{{x + c_size/2 - 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y - c_size/2 + 2*dist},
                                      {x - c_size/2 + 2*dist, y + c_size/2 - dist},{x - c_size/2 + dist, y + c_size/2 - 2*dist}};
    rectangle_2->set_color(Graph_lib::Color::red);
    rectangle_2->set_fill_color(Graph_lib::Color::red);

    chess = &chess_;
}

RedCross::~RedCross()
{
    //Graph_lib::Rectangle::~Rectangle();
    chess->detach(*rectangle_1);
    delete rectangle_1;
    chess->detach(*rectangle_2);
    delete rectangle_2;
    chess->detach(*this);
}

void RedCross::draw_lines() const
{
    rectangle_1->draw_lines();
    rectangle_2->draw_lines();
}

Frame::Frame(Point center, Chessboard& chess_) : 
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

    //There are probably ways to avoid Copy+Paste but there's 
    //something with Graph_lib that doesn't allow it to be easy

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

    chess = &chess_;
}

//I am very unsure of this destructor
Frame::~Frame()
{
    for(int i = (int)horisontal_rectangles.size() - 1; i >= 0; i--)
    {
        //Graph_lib::Rectangle::~Rectangle();
        chess->detach(*horisontal_rectangles[i]);
        delete horisontal_rectangles[i];
        horisontal_rectangles.erase(horisontal_rectangles.begin()+i,horisontal_rectangles.begin()+i+1);
        chess->detach(*vertical_rectangles[i]);
        delete vertical_rectangles[i];
        vertical_rectangles.erase(vertical_rectangles.begin()+i,vertical_rectangles.begin()+i+1);
    }
    chess->detach(*this);
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

//I am very unsure of this destructor
VisualSteps::~VisualSteps()
{
    for(int i = int(possible_steps.size() - 1); i >= 0; i--)
    {
        chess->detach(*possible_steps[i]);
        delete possible_steps[i];
        possible_steps.erase(possible_steps.begin()+i,possible_steps.begin()+i+1);
    }
    for(int i = int(possible_takes.size()-1); i >= 0; i--)
    {
        chess->detach(*possible_takes[i]);
        delete possible_takes[i];
        possible_takes.erase(possible_takes.begin()+i,possible_takes.begin()+i+1);
    }
}