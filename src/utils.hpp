#ifndef __UTILS__
#define __UTILS__

#include <ostream>

/*
Contient des struct et foncitons utiles un peu partout dans le code
*/

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