#include "bloc.hpp"
#include "utils.hpp"

Bloc::Bloc() :
    m_data(set_data(0, 0, false, Orientation::horizontal))
{}

Bloc::Bloc(char _x, char _y, bool _size, Orientation _orientation) :
    m_data(set_data(_x, _y, _size, _orientation))
{}

Bloc::Bloc(const Bloc& b) :
    m_data(b.m_data)
{}

Bloc::Bloc(Bloc&& b) :
    m_data(b.m_data)
{
    b.m_data = 0;
}

Bloc& Bloc::operator=(const Bloc& b)
{
    m_data = b.m_data;
    return *this;
}

uint2 Bloc::get_coord() const {
    return uint2(
        (m_data & 0b11100000) >> 5,
        (m_data & 0b00011100) >> 2
    );
}

uint8_t Bloc::get_size() const {
    return ((m_data & 0b00000010) == 2 ? 3 : 2);
}

Orientation Bloc::get_orientation() const {
    return (m_data & 0b00000001) == 0 ? Orientation::horizontal : Orientation::vertical;
}

char Bloc::get_raw() const {
    return m_data;
}

char& Bloc::set_data(char _x, char _y, bool _size, Orientation _orientation) {
    m_data = _x;
    m_data <<= 3;
    m_data += _y;
    m_data <<= 1;
    m_data += _size & 0b1;
    m_data <<= 1;
    m_data += (_orientation == Orientation::horizontal ? 0 : 1);
    return m_data;
}

char& Bloc::set_data(char raw_data) {
    m_data = raw_data;
    return m_data;
}

void Bloc::set_coord(uint2 new_coord) {
    char data = new_coord.x;
    data <<=3;
    data += new_coord.y;
    data <<=2;
    m_data = data + (m_data & 0b00000011);
}

std::ostream& operator<<(std::ostream& os, Bloc& a) {
    os << a.m_data;
    return os;
}

bool Bloc::test() {
    int nb_erreur = 0;

    uint2 coord(0, 0);
    uint8_t size;
    Orientation o;

    Bloc b(0, 0, false, Orientation::horizontal);
    if((b.m_data & 0x0000) != 0x0000) {
        std::cout << "Erreur dans la représentation binaire test 1" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;
        
        nb_erreur++;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 0 || coord.y != 0 || size != 2 || o != Orientation::horizontal) {
        std::cout << "Erreur dans les getter test 1" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        nb_erreur++;
    }



    b.set_data(7, 7, true, Orientation::vertical);
    if((b.m_data & 0xffff) != 0xffff) {
        std::cout << "Erreur dans la représentation binaire test 2" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        nb_erreur++;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 7 || coord.y != 7 || size != 3 || o != Orientation::vertical) {
        std::cout << "Erreur dans les getter test 2" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        nb_erreur++;
    }



    b.set_data(5, 3, false, Orientation::vertical);
    if((b.m_data & 0b10101100) != 0b10101100) {
        std::cout << "Erreur dans la représentation binaire test 3" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        nb_erreur++;
    }

    coord = b.get_coord();
    size = b.get_size();
    o = b.get_orientation();

    if(coord.x != 5 || coord.y != 3 || size != 2 || o != Orientation::vertical) {
        std::cout << "Erreur dans les getter test 3" << "\n"; 
        std::cout << "   " << b.get_coord() << " " << b.get_size() << " " << (b.get_orientation() == Orientation::horizontal ? "h" : "v") << "\n";
        std::cout << "   " << "Binaire: " << human_readable(b.m_data) << std::endl;

        nb_erreur++;
    }

    b.set_coord(uint2(4, 2));
    coord = b.get_coord();
    if(coord.x != 4 || coord.y != 2) {
        std::cout << "Erreur dans set coord: (4, 2) attendu, mais " << coord << " lu.\n";
        nb_erreur++;
    }

    if(nb_erreur != 0) {
        std::cout << nb_erreur << " erreurs dans bloc\n";
        return false;
    }
    return true;
}