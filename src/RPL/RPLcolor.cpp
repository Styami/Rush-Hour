#include "RPLcolor.hpp"
#include <string>

using namespace RPL;

Color::Color() {
    r = g = b = 0;
}

Color::Color(unsigned char _r, unsigned char _g, unsigned char _b) :
    r(_r), g(_g), b(_b)
{
}

Color::~Color() {

}

float Color::get_intensity() const {
    return std::sqrt(r*r + g*g + b*b);
}

std::string Color::get_ansi_code() const {
    return "\e[38;2;" + std::to_string(r) + ";" + std::to_string(r) + ";" + std::to_string(r) + "m";
}