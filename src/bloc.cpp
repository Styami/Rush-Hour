#include "bloc.hpp"
#include "utils.hpp"

Bloc::Bloc() :
    m_data(set_data(0, 0, false, Orientation::horizontal))
{}

Bloc::Bloc(char _x, char _y, bool _size, Orientation _orientation) :
    m_data(set_data(_x, _y, _size, _orientation))
{
}
    
int2 Bloc::get_coord() const {
    return int2(
        (m_data & 0b11100000) >> 4,
        (m_data & 0b00011100) >> 2
    );
}

int Bloc::get_size() const {
    return (m_data & 0b00000010) >> 1;
}

Orientation Bloc::get_orientation() const {
    return (m_data & 0b00000001) == 1 ? Orientation::horizontal : Orientation::vertical;
}

char& Bloc::set_data(char _x, char _y, bool _size, Orientation _orientation) {
    m_data = _x;
    m_data <<= 3;
    m_data += _y;
    m_data <<= 1;
    m_data += _size & 0b1;
    m_data <<= 1;
    m_data += _orientation == Orientation::horizontal ? 0 : 1;
    return m_data;
}

std::ostream& operator<<(std::ostream& os, Bloc& a) {
    os << a.m_data;
    return os;
}

bool Bloc::test() {
    int2 coord(0, 0);
    bool size;
    Orientation o;

    Bloc b(0, 0, false, Orientation::horizontal);
    if((b.m_data & 0x0000) != 0x0000) {
        std::cout << "Erreur dans la représentation binaire test 1" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;
        
        return false;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 0 && coord.y != 0 && size != false && o != Orientation::horizontal) {
        std::cout << "Erreur dans les getter test 1" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        return false;
    }



    b.set_data(7, 7, true, Orientation::vertical);
    if((b.m_data & 0xffff) != 0xffff) {
        std::cout << "Erreur dans la représentation binaire test 2" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        return false;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 7 && coord.y != 7 && size != true && o != Orientation::vertical) {
        std::cout << "Erreur dans les getter test 2" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        return false;
    }



    b.set_data(5, 3, false, Orientation::vertical);
    if((b.m_data & 0b10101100) != 0b10101100) {
        std::cout << "Erreur dans la représentation binaire test 3" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        return false;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 5 && coord.y != 3 && size != false && o != Orientation::vertical) {
        std::cout << "Erreur dans les getter test 3" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        return false;
    }


    return true;
}