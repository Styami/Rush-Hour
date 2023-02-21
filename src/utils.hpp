#ifndef __UTILS__
#define __UTILS__

#include <iostream>

struct int2 {
    int x, y;

    int2(int x_, int y_);

    friend std::ostream& operator<<(std::ostream& os, const int2& a);
};

#endif