#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <ostream>
#include <math.h>

struct int2 {
    int x, y;

    int2(int x_, int y_);

    friend std::ostream& operator<<(std::ostream& os, const int2& a);
};

int human_readable(const char& _bit_char);

#endif