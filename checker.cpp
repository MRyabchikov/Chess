#include "cell.h"
#include "checker.h"


void Checker::attach(const Cell& c) {
    move(c.center().x - center().x, c.center().y - center().y);
    cell = &c;
}