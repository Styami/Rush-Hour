#include "plateau.hpp"

Plateau::Plateau(std::size_t nb_blocks) :
    m_blocks_array(new Bloc[nb_blocks]), m_blocks_count(nb_blocks)
{}

Plateau::Plateau(std::string file_path) :
    m_blocks_array(load_file(file_path)), 
    m_blocks_count(get_block_count_from_file(file_path))
{
}

Plateau::~Plateau() {
    delete [] m_blocks_array;
}

Bloc* Plateau::load_file(std::string file_path) {
    // TODO: implémenter lecture de fichier
    return nullptr;
}

std::size_t Plateau::get_block_count_from_file(std::string file_path) {
    // TODO: implémenter lecture nombre de bloc
    return 0;
}

bool Plateau::can_block_move(const Bloc& block, int displacement) const {
    int2 coords = block.get_coord();
    Orientation o = block.get_orientation();
    uint8_t size = block.get_size();

    switch(o) {
    case Orientation::horizontal:
        coords.x += displacement + (displacement > 0 ? size - 1 : 0);
        break;
    case Orientation::vertical:
        coords.y += displacement + (displacement > 0 ? size - 1 : 0);
        break;
    }

    return is_cell_empty(coords);
}

bool Plateau::is_cell_empty(int2 coords) const {
    int2 block_coords;
    uint8_t block_size;
    for(int i = 0; i < m_blocks_count; i++) {
        block_coords = m_blocks_array[i].get_coord();
        block_size = m_blocks_array[i].get_size();

        switch(m_blocks_array[i].get_orientation()) {
        case Orientation::horizontal:
            if(block_coords.y == coords.y && block_coords.x <= coords.x && block_coords.x + block_size >= coords.x)
                return false;
            break;
        case Orientation::vertical:
            if(block_coords.x == coords.x && block_coords.y <= coords.y && block_coords.y + block_size >= coords.y)
                return false;
            break;
        }
    }
    return true;
}

std::unique_ptr<Plateau> Plateau::move_block(const Bloc& block, int displacement) const {
    std::unique_ptr<Plateau> new_board;

    // TODO: implémenter la génération d'un nouveau plateau

    return new_board;
}

bool Plateau::test() {
    // TODO: implémenter la fonction de test
    return true;
}