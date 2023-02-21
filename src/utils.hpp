#ifndef __UTILS__
#define __UTILS__

#include <iostream>

struct int2 {
    int x, y;

    friend std::ostream& operator<<(std::ostream& os, const int2& a);
};

#endif