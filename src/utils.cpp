#include "utils.hpp"
/*
Contient des struct et foncitons utiles un peu partout dans le code
*/

std::ostream& operator<<(std::ostream& os, const int2& a) {
    os << "(" << a.x << ", " << a.y << ")";
    return os;
}

int2::int2() :
    x(0), y(0)
{}

int2::int2(int x_, int y_) :
    x(x_), y(y_)
{}

int2 int2::operator+(int2 a) {
    return (int2){x + a.x, y + a.y};
}

int2 int2::operator-(int2 a) {
    return (int2){x - a.x, y - a.y};
}

int2 int2::operator*(int2 a) {
    return (int2){x * a.x, y * a.y};
}

int2 int2::operator/(int2 a) {
    return (int2){x / a.x, y / a.y};
}

int2 int2::operator*(int a) {
    return (int2){x * a, y * a};
}

int2 int2::operator/(int a) {
    return (int2){x / a, y / a};
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