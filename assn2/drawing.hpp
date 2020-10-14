#ifndef DRAWING_HPP
#define DRAWING_HPP

#include "geometry_objects.hpp"

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int computeOutcode(Point, Point, Point);
int clipLine(Point&, Point&, Window);
std::vector<Point> drawLine(Point, Point);

#endif