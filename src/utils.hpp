#ifndef __UTILS__
#define __UTILS__

#include <iostream>
#include <ostream>
#include <math.h>

struct uint2 {
    unsigned int x, y;

    uint2();
    uint2(unsigned int x_, unsigned int y_);

    uint2 operator+(uint2 a);
    uint2 operator-(uint2 a);
    uint2 operator*(uint2 a);
    uint2 operator/(uint2 a);
    uint2 operator*(unsigned int a);
    uint2 operator/(unsigned int a);

    friend std::ostream& operator<<(std::ostream& os, const uint2& a);
};

int human_readable(const char& _bit_char);

#endif