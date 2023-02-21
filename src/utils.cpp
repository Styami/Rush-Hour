#include "utils.hpp"
#include <ostream>
/*
Contient des struct et foncitons utiles un peu partout dans le code
*/

std::ostream& operator<<(std::ostream& os, const int2& a) {
    os << "(" << a.x << ", " << a.y << ")";
    return os;
}

int2::int2(int x_, int y_) {
    x = x_;
    y = y_;
}