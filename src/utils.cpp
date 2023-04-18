#include "utils.hpp"

#include <iostream>
#include <math.h>

std::ostream& operator<<(std::ostream& os, const uint2& a) {
    os << "(" << a.x << ", " << a.y << ")";
    return os;
}

uint2::uint2() :
    x(0), y(0)
{}

uint2::uint2(unsigned int x_, unsigned int y_) :
    x(x_), y(y_)
{}

uint2 uint2::operator+(uint2 a) {
    return (uint2){x + a.x, y + a.y};
}

uint2 uint2::operator-(uint2 a) {
    return (uint2){x - a.x, y - a.y};
}

uint2 uint2::operator*(uint2 a) {
    return (uint2){x * a.x, y * a.y};
}

uint2 uint2::operator/(uint2 a) {
    return (uint2){x / a.x, y / a.y};
}

uint2 uint2::operator*(unsigned int a) {
    return (uint2){x * a, y * a};
}

uint2 uint2::operator/(unsigned int a) {
    return (uint2){x / a, y / a};
}

int human_readable(const char& _bit_char) {
    int ret = 0;
    unsigned char mask = 0b10000000;

    for(int i = 0; i < 8; i++) {
        ret *= 10;
        ret += (_bit_char & mask) >> (7 - i);
        mask >>= 1;
    }

    return ret;
}