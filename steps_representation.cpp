#include "steps_representation.h"
#include "board.h"


DangerSign::DangerSign(Point center, Simple_window& chess_) : Circle{center, c_size/2}
{
    for(int i = center.x - c_size/4; i <= center.x + c_size/4; i += c_size/2)
    {
        for(int j = center.y - c_size/4; j <= center.y + c_size/4; j += c_size/2)
        {
            Circle* temporary_circle = new Circle{{i,j}, circle_radius};
            AttachManager<Circle> am_temporary_circle{temporary_circle, chess_};

            circle_of_circles.push_back(am_temporary_circle);
            circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
            circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
        }
    }
    for(int i = center.x - c_size/4 - c_size/8; i <= center.x + c_size/4 + c_size/8; i += c_size/4 + c_size/8)
    {
        Circle* temporary_circle = new Circle{{i,center.y}, circle_radius};
        AttachManager<Circle> am_temporary_circle{temporary_circle, chess_};

        circle_of_circles.push_back(am_temporary_circle);
        circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
        circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
    }
    for(int j = center.y - c_size/4 - c_size/8; j <= center.y + c_size/4 + c_size/8; j += c_size/2 + c_size/4)
    {
        Circle* temporary_circle = new Circle{{center.x,j}, circle_radius};
        AttachManager<Circle> am_temporary_circle{temporary_circle, chess_};

        circle_of_circles.push_back(am_temporary_circle);
        circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
        circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
    }
}

void DangerSign::draw_lines() const
{
    for(int i = 0; i < int(circle_of_circles.size()); i++)
        circle_of_circles[i].draw_lines();
}

RedCross::RedCross(Point center, Simple_window& chess_) : Rectangle{center, c_size, c_size}
{
    int x = center.x;
    int y = center.y;

    Closed_polyline* rectangle_1 = new Closed_polyline{{x - c_size/2 + 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y + c_size/2 - 2*dist},
                                      {x + c_size/2 - 2*dist, y + c_size/2 - dist}, {x - c_size/2 + dist, y - c_size/2 + 2*dist}};
    AttachManager<Closed_polyline> am_rectangle_1{rectangle_1, chess_};

    am_rectangle_1.set_color(Graph_lib::Color::red);
    am_rectangle_1.set_fill_color(Graph_lib::Color::red);

    Closed_polyline* rectangle_2 = new Closed_polyline{{x + c_size/2 - 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y - c_size/2 + 2*dist},
                                      {x - c_size/2 + 2*dist, y + c_size/2 - dist},{x - c_size/2 + dist, y + c_size/2 - 2*dist}};
    AttachManager<Closed_polyline> am_rectangle_2{rectangle_2, chess_};

    am_rectangle_2.set_color(Graph_lib::Color::red);
    am_rectangle_2.set_fill_color(Graph_lib::Color::red);

    rectangles.push_back(am_rectangle_1);
    rectangles.push_back(am_rectangle_2);
}

void RedCross::draw_lines() const
{
    rectangles[0].draw_lines();
    rectangles[1].draw_lines();
}

Frame::Frame(Point center, Simple_window& chess_) : 
    Rectangle({center.x - c_size/2, center.y - c_size/2}, c_size, c_size)
{

    Rectangle* h_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_width, rc_length};
    Rectangle* h_r2 = new Rectangle{{center.x + c_size/2 - rc_width, center.y - c_size/2}, rc_width, rc_length};
    Rectangle* h_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_length}, rc_width, rc_length};
    Rectangle* h_r4 = new Rectangle{{center.x + c_size/2 - rc_width, center.y + c_size/2 - rc_length}, rc_width, rc_length};

    AttachManager<Rectangle> am_h_r1{h_r1, chess_};
    AttachManager<Rectangle> am_h_r2{h_r2, chess_};
    AttachManager<Rectangle> am_h_r3{h_r3, chess_};
    AttachManager<Rectangle> am_h_r4{h_r4, chess_};

    horisontal_rectangles.push_back(am_h_r1);
    horisontal_rectangles.push_back(am_h_r2);
    horisontal_rectangles.push_back(am_h_r3);
    horisontal_rectangles.push_back(am_h_r4);

    //There are probably ways to avoid Copy+Paste but there's 
    //something with Graph_lib that doesn't allow it to be easy

    Rectangle* v_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_length, rc_width};
    Rectangle* v_r2 = new Rectangle{{center.x + c_size/2 - rc_length, center.y - c_size/2}, rc_length, rc_width};
    Rectangle* v_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_width}, rc_length, rc_width};
    Rectangle* v_r4 = new Rectangle{{center.x + c_size/2 - rc_length, center.y + c_size/2 - rc_width}, rc_length, rc_width};

    AttachManager<Rectangle> am_v_r1{v_r1, chess_};
    AttachManager<Rectangle> am_v_r2{v_r2, chess_};
    AttachManager<Rectangle> am_v_r3{v_r3, chess_};
    AttachManager<Rectangle> am_v_r4{v_r4, chess_};

    vertical_rectangles.push_back(am_v_r1);
    vertical_rectangles.push_back(am_v_r2);
    vertical_rectangles.push_back(am_v_r3);
    vertical_rectangles.push_back(am_v_r4);

    for(int i = 0; i < 4; i++)
    {
        horisontal_rectangles[i].set_color(chess_yellow);
        horisontal_rectangles[i].set_fill_color(chess_yellow);

        vertical_rectangles[i].set_color(chess_yellow);
        vertical_rectangles[i].set_fill_color(chess_yellow);
    }
}

void Frame::draw_lines() const
{
    for(int i = 0; i < 4; i++)
    {
        horisontal_rectangles[i].draw_lines();
        vertical_rectangles[i].draw_lines();
    }
}