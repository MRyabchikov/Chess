#include "steps_representation.h"
#include "board.h"


template<typename T>
Unique_attacher<T>::Unique_attacher(T* object_, Chessboard& chess_) : object{object_}, chess{&chess_}
{
    chess->attach(*object_);
}

template<typename T>
Unique_attacher<T>::Unique_attacher(const T& ua)
{
    object = ua.object;
    chess = ua.chess;
    ua.object = nullptr;
}

template<typename T>
Unique_attacher<T>::~Unique_attacher()
{
    if(object != nullptr)
    {
        chess->detach(*object);
        delete object;
    }
}

template<typename T>
Unique_attacher<T>& Unique_attacher<T>::operator=(const T& ua)

{
    if(object != nullptr)
    {
        chess->detach(*object);
        delete object;
    }
    object = ua.object;
    chess = ua.chess;
    ua.object = nullptr;
}

template<typename T>
T& Unique_attacher<T>::obj()
{
    if(object != nullptr)
        return *object;
    return nullptr;
}

template<typename T>
void Unique_attacher<T>::draw_lines() const
{
    if(object != nullptr)
        object->draw_lines();
}

template<typename T>
void Unique_attacher<T>::set_color(int color) const
{
    if(object != nullptr)
        object->set_color(color);
}

template<typename T>
void Unique_attacher<T>::set_fill_color(int color) const
{
    if(object != nullptr)
        object->set_fill_color(color);
}

void Detacher::detach(Shape& fig) { chess->detach(fig); }

DangerSign::DangerSign(Point center, Chessboard& chess_) : Circle{center, c_size/2}, det{chess_}
{
    for(int i = center.x - c_size/4; i <= center.x + c_size/4; i += c_size/2)
    {
        for(int j = center.y - c_size/4; j <= center.y + c_size/4; j += c_size/2)
        {
            Circle* temporary_circle = new Circle{{i,j}, circle_radius};
            circle_of_circles.push_back(temporary_circle);
            circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
            circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
        }
    }
    for(int i = center.x - c_size/4 - c_size/8; i <= center.x + c_size/4 + c_size/8; i += c_size/4 + c_size/8)
    {
        Circle* temporary_circle = new Circle{{i,center.y}, circle_radius};
        circle_of_circles.push_back(temporary_circle);
        circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
        circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
    }
    for(int j = center.y - c_size/4 - c_size/8; j <= center.y + c_size/4 + c_size/8; j += c_size/2 + c_size/4)
    {
        Circle* temporary_circle = new Circle{{center.x,j}, circle_radius};
        circle_of_circles.push_back(temporary_circle);
        circle_of_circles[circle_of_circles.size()-1].set_fill_color(Graph_lib::Color::red);
        circle_of_circles[circle_of_circles.size()-1].set_color(Graph_lib::Color::red);
    }
}

DangerSign::~DangerSign()
{
    for(int i = (int)circle_of_circles.size() - 1; i >= 0; i--)
        det.detach(circle_of_circles[i]);
    det.detach(*this);
}

void DangerSign::draw_lines() const
{
    for(int i = 0; i < int(circle_of_circles.size()); i++)
        circle_of_circles[i].draw_lines();
}

RedCross::RedCross(Point center, Chessboard& chess_) : Rectangle{center, c_size, c_size}, det{chess_}
{
    int x = center.x;
    int y = center.y;

    Closed_polyline* rectangle_1 = new Closed_polyline{{x - c_size/2 + 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y + c_size/2 - 2*dist},
                                      {x + c_size/2 - 2*dist, y + c_size/2 - dist}, {x - c_size/2 + dist, y - c_size/2 + 2*dist}};
    rectangle_1->set_color(Graph_lib::Color::red);
    rectangle_1->set_fill_color(Graph_lib::Color::red);

    Closed_polyline* rectangle_2 = new Closed_polyline{{x + c_size/2 - 2*dist, y - c_size/2 + dist},{x + c_size/2 - dist, y - c_size/2 + 2*dist},
                                      {x - c_size/2 + 2*dist, y + c_size/2 - dist},{x - c_size/2 + dist, y + c_size/2 - 2*dist}};
    rectangle_2->set_color(Graph_lib::Color::red);
    rectangle_2->set_fill_color(Graph_lib::Color::red);

    rectangles.push_back(rectangle_1);
    rectangles.push_back(rectangle_2);
}

RedCross::~RedCross()
{
    det.detach(rectangles[0]);
    det.detach(rectangles[1]);
    det.detach(*this);
}

void RedCross::draw_lines() const
{
    rectangles[0].draw_lines();
    rectangles[1].draw_lines();
}

Frame::Frame(Point center, Chessboard& chess_) : 
    Rectangle({center.x - c_size/2, center.y - c_size/2}, c_size, c_size)
{
    /*
    Unique_attacher<Rectangle> h_r1{new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_width, rc_length}, chess_};
    Unique_attacher<Rectangle> h_r2{new Rectangle{{center.x + c_size/2 - rc_width, center.y - c_size/2}, rc_width, rc_length}, chess_};
    Unique_attacher<Rectangle> h_r3{new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_length}, rc_width, rc_length}, chess_};
    Unique_attacher<Rectangle> h_r4{new Rectangle{{center.x + c_size/2 - rc_width, center.y + c_size/2 - rc_length}, rc_width, rc_length}, chess_};
    */

    Rectangle* h_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_width, rc_length};
    Rectangle* h_r2 = new Rectangle{{center.x + c_size/2 - rc_width, center.y - c_size/2}, rc_width, rc_length};
    Rectangle* h_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_length}, rc_width, rc_length};
    Rectangle* h_r4 = new Rectangle{{center.x + c_size/2 - rc_width, center.y + c_size/2 - rc_length}, rc_width, rc_length};

    Unique_attacher<Rectangle> ua_h_r1{h_r1, chess_};
    Unique_attacher<Rectangle> ua_h_r2{h_r2, chess_};
    Unique_attacher<Rectangle> ua_h_r3{h_r3, chess_};
    Unique_attacher<Rectangle> ua_h_r4{h_r4, chess_};

    horisontal_rectangles.push_back(ua_h_r1);
    horisontal_rectangles.push_back(ua_h_r2);
    horisontal_rectangles.push_back(ua_h_r3);
    horisontal_rectangles.push_back(ua_h_r4);

    /*
    horisontal_rectangles.emplace_back(h_r1, chess_);
    horisontal_rectangles.emplace_back(h_r2, chess_);
    horisontal_rectangles.emplace_back(h_r3, chess_);
    horisontal_rectangles.emplace_back(h_r4, chess_);
    */

    //There are probably ways to avoid Copy+Paste but there's 
    //something with Graph_lib that doesn't allow it to be easy

    /*
    Unique_attacher<Rectangle> v_r1{new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_length, rc_width}, chess_};
    Unique_attacher<Rectangle> v_r2{new Rectangle{{center.x + c_size/2 - rc_length, center.y - c_size/2}, rc_length, rc_width}, chess_};
    Unique_attacher<Rectangle> v_r3{new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_width}, rc_length, rc_width}, chess_};
    Unique_attacher<Rectangle> v_r4{new Rectangle{{center.x + c_size/2 - rc_length, center.y + c_size/2 - rc_width}, rc_length, rc_width}, chess_};
    */

    Rectangle* v_r1 = new Rectangle{{center.x - c_size/2, center.y - c_size/2}, rc_length, rc_width};
    Rectangle* v_r2 = new Rectangle{{center.x + c_size/2 - rc_length, center.y - c_size/2}, rc_length, rc_width};
    Rectangle* v_r3 = new Rectangle{{center.x - c_size/2, center.y + c_size/2 - rc_width}, rc_length, rc_width};
    Rectangle* v_r4 = new Rectangle{{center.x + c_size/2 - rc_length, center.y + c_size/2 - rc_width}, rc_length, rc_width};

    Unique_attacher<Rectangle> ua_v_r1{v_r1, chess_};
    Unique_attacher<Rectangle> ua_v_r2{v_r2, chess_};
    Unique_attacher<Rectangle> ua_v_r3{v_r3, chess_};
    Unique_attacher<Rectangle> ua_v_r4{v_r4, chess_};

    vertical_rectangles.push_back(ua_v_r1);
    vertical_rectangles.push_back(ua_v_r2);
    vertical_rectangles.push_back(ua_v_r3);
    vertical_rectangles.push_back(ua_v_r4);

    /*
    vertical_rectangles.emplace_back(v_r1, chess_);
    vertical_rectangles.emplace_back(v_r2, chess_);
    vertical_rectangles.emplace_back(v_r3, chess_);
    vertical_rectangles.emplace_back(v_r4, chess_);
    */

    for(int i = 0; i < 4; i++)
    {
        /*
        Rectangle& hr = (const_cast<Unique_attacher<Rectangle>*>(&horisontal_rectangles[i]))->obj();
        Rectangle& vr = (const_cast<Unique_attacher<Rectangle>*>(&vertical_rectangles[i]))->obj();

        hr.set_color(chess_yellow);
        hr.set_fill_color(chess_yellow);

        vr.set_color(chess_yellow);
        vr.set_fill_color(chess_yellow);
        */

        horisontal_rectangles[i].set_color(chess_yellow);
        horisontal_rectangles[i].set_fill_color(chess_yellow);

        vertical_rectangles[i].set_color(chess_yellow);
        vertical_rectangles[i].set_fill_color(chess_yellow);
    }
}

/*
Frame::~Frame()
{
    for(int i = (int)horisontal_rectangles.size() - 1; i >= 0; i--)
    {
        det.detach(horisontal_rectangles[i]);
        det.detach(vertical_rectangles[i]);
    }
    det.detach(*this);
}
*/

void Frame::draw_lines() const
{
    for(int i = 0; i < 4; i++)
    {
        /*
        Rectangle& hr = (const_cast<Unique_attacher<Rectangle>*>(&horisontal_rectangles[i]))->obj();
        Rectangle& vr = (const_cast<Unique_attacher<Rectangle>*>(&vertical_rectangles[i]))->obj();

        hr.draw_lines();
        vr.draw_lines();
        */

       horisontal_rectangles[i].draw_lines();
       vertical_rectangles[i].draw_lines();
    }
}

VisualSteps::~VisualSteps()
{
    for(int i = int(possible_steps.size() - 1); i >= 0; i--)
        det.detach(possible_steps[i]);
}