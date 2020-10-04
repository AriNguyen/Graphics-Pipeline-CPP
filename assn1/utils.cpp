#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <cmath>
#include "utils.h"

#define PI 3.14259

// Specs
Specs::Specs(int argc, char *argv[]) {
    // default
    fileName = (char *)"hw1.ps";
    window.width = 499;
    window.height = 499;
    scaleFactor = 1.0;
    rotateDegree = 1;
    xDim = 0; 
    yDim = 0;
    
    // parse argv
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) 
            fileName = argv[++i];
        else if (strcmp(argv[i], "-s") == 0) 
            scaleFactor = atof(argv[++i]);
        else if (strcmp(argv[i], "-r") == 0) 
            rotateDegree = atoi(argv[++i]);
        else if (strcmp(argv[i], "-m") == 0) 
            xDim = std::atoi(argv[++i]);
        else if (strcmp(argv[i], "-n") == 0) 
            yDim = std::atoi(argv[++i]);
        else if (strcmp(argv[i], "-a") == 0) 
            lowX = std::atoi(argv[++i]);
        else if (strcmp(argv[i], "-b") == 0) 
            lowY = std::atoi(argv[++i]);
        else if (strcmp(argv[i], "-c") == 0) 
            upX  = std::atoi(argv[++i]);
        else if (strcmp(argv[i], "-d") == 0) 
            upY = std::atoi(argv[++i]);
    }
    // printf("Window %d %d %d %d \n", lowX, lowY, upX, upY);
    window.loadDim(lowX, lowY, upX, upY);
}


void Window::loadDim(int lowX, int lowY, int upX, int upY) {
    // printf("loadim %d %d %d %d \n", lowX, lowY, upX, upY);
    lowBound.x = lowX;
    lowBound.y = lowY;
    upBound.x = upX;
    upBound.y = upY;
    width = upX - lowX;
    height = upY - lowY;
    // printf("loadim w h: %d %d\n", width, height);
}


int computeOutcode(Point point, Point lowBound, Point upBound) {
    int code = INSIDE;              // initialized to be inside window
    if (point.x < lowBound.x)       // to the left
        code |= LEFT;
    else if (point.x > upBound.x)   // to the right
        code |= RIGHT;
    else if (point.y < lowBound.y)  // below window
        code |= BOTTOM;
    else if (point.y > upBound.y)   // above window
        code |= TOP;
    return code;
}

// cohenSutherland: clip line
// Get 2 points and window size
// Return 0: if both points are outside
// Otherwise return 1;
int clipLine(Point &p0, Point &p1, Window win) {
    int outCode0 = computeOutcode(p0, win.lowBound, win.upBound);
    int outCode1 = computeOutcode(p1, win.lowBound, win.upBound);
    // printf("win: %d %d, %d %d\n", win.lowBound.x, win.lowBound.y, win.upBound.x, win.upBound.y);
    // printf("points: %d %d, %d %d\n", p0.x, p0.y, p1.x, p1.y);
    // printf("outCode1 outCode2: %d %d\n", outCode0, outCode1);

    while (true) {
        if (!(outCode0 | outCode1)) {      // both points inside widnow
            break;
        } else if (outCode0 & outCode1) {  // both outside window
            return 0;
        } else {                            // either of 2 points out side window
            int outCode = outCode1 > outCode0 ? outCode1 : outCode0;    // get larger one
            int x, y;

            if (outCode & TOP) {           // point is above the clip window
				x = p0.x + (p1.x - p0.x) * (win.upBound.y - p0.y) / (p1.y - p0.y);
				y = win.upBound.y;
			} else if (outCode & BOTTOM) { // point is below the clip window
				x = p0.x + (p1.x - p0.x) * (win.lowBound.y - p0.y) / (p1.y - p0.y);
				y = win.lowBound.y ;
			} else if (outCode & RIGHT) {  // point is to the right of clip window
				y = p0.y + (p1.y - p0.y) * (win.upBound.x - p0.x) / (p1.x - p0.x);
				x = win.upBound.x;
			} else if (outCode & LEFT) {   // point is to the left of clip window
				y = p0.y + (p1.y - p0.y) * (win.lowBound.x - p0.x) / (p1.x - p0.x);
				x = win.lowBound.x ;
			}

            if (outCode == outCode0) {
				p0.x = x;
				p0.y = y;
				outCode0 = computeOutcode(p0, win.lowBound, win.upBound);
			} else {
				p1.x = x;
				p1.y = y;
				outCode1 = computeOutcode(p1, win.lowBound, win.upBound);
			}
        }
    }
    return 1;
}

/* bresenham algorithm: draw to stdout
* Given 2 points of the line
* Return vector of points lie in the line
*/
std::vector<Point> drawLine(Point p0, Point p1) {
    std::vector<Point> points;
    int dx = abs(p1.x - p0.x);
    int sx = p0.x < p1.x ? 1 : -1;
    int dy = -abs(p1.y - p0.y);
    int sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy;  
    // printf("drawLine: %d %d, %d %d\n", p0.x, p0.y, p1.x, p1.y);
    while (true) {  
        // printf(" x y: %d %d\n", p0.x, p0.y);
        Point p {p0.x, p0.y};
        points.push_back(p);
        if (p0.x == p1.x && p0.y == p1.y) 
            break;
        int e2 = 2*err;
        if (e2 >= dy) { 
            err += dy;
            p0.x += sx;
        }
        if (e2 <= dx) {
            err += dx;
            p0.y += sy;
        }
    }
    return points;
}

std::vector<std::string> tokenizeBySymbol(std::string str, char symbol) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string cell;
    while(std::getline(ss, cell, symbol)) {
        tokens.push_back(cell);
        // std::cout << cell << std::endl;   
    }
    return tokens;
} 

void scale(int &x, int &y, float scaleFactor) {
    x = round(x * scaleFactor); 
    y = round(y * scaleFactor);
}

void rotate(int &x, int &y, int rotateDegree) {
    int x0 = x;
    int y0 = y;
    double rad = rotateDegree * PI/180;
    // std::cout << "rad: " << rad << " " << round(cos(rad)) << " "  << round(sin(rad)) << std::endl;
    // printf("x0 y0: %d %d\n", x0, y0);
    x = x0*round(cos(rad)) - y0*round(sin(rad));
    y = x0*round(sin(rad)) + y0*round(cos(rad));
    // printf("x y: %d %d\n", x, y);

}

void translate(int &x, int &y, int dx, int dy) {
    x += dx;
    y += dy;
}

