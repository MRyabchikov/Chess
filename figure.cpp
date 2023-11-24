#include "figure.h"
#include "cell.h"

void Figure::attach(const Cell& c)
{
    move(c.center().x - point(0).x - 40, c.center().y - point(0).y - 45);
    cell = &c;
}