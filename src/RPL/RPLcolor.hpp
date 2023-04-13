#ifndef __RPL_COLOR__
#define __RPL_COLOR__

#include <math.h>
#include <string>

namespace RPL {

class Color {
public:
    unsigned char r, g, b;

    Color(unsigned char _r, unsigned char _g, unsigned char _b);
    Color();
    ~Color();

    float get_intensity() const;

    //Ne marche pas ?
    std::string get_ansi_code() const;

};

}

#endif