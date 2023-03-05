#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <ostream>
#include <math.h>

struct int2 {
    int x, y;

    int2();
    int2(int x_, int y_);

    int2 operator+(int2 a);
    int2 operator-(int2 a);
    int2 operator*(int2 a);
    int2 operator/(int2 a);
    int2 operator*(int a);
    int2 operator/(int a);

    friend std::ostream& operator<<(std::ostream& os, const int2& a);
};

int human_readable(const char& _bit_char);

#endif