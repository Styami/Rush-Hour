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

    int offset = displacement + (displacement > 0 ? block.get_size() - 1 : 0);
    if(block.get_orientation() == Orientation::horizontal)
        coords.x += offset;
    else
        coords.y += offset;

    return is_cell_empty(coords);
}

bool Plateau::is_cell_empty(int2 coords) const {
    int2 block_coords;
    uint8_t block_size;
    for(std::size_t i = 0; i < m_blocks_count; i++) {
        block_coords = m_blocks_array[i].get_coord();
        block_size = m_blocks_array[i].get_size();

        switch(m_blocks_array[i].get_orientation()) {
        case Orientation::horizontal:
            if(block_coords.y == coords.y && block_coords.x <= coords.x && block_coords.x + block_size > coords.x)
                return false;
            break;
        case Orientation::vertical:
            if(block_coords.x == coords.x && block_coords.y <= coords.y && block_coords.y + block_size > coords.y)
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
    int nb_erreur = 0;

    Plateau p(2);
    bool test_map[36];
    bool blocs_map[36]{0};

    p.m_blocks_array[0].set_data(1, 2, false, Orientation::horizontal);
    p.m_blocks_array[1].set_data(4, 2, true, Orientation::vertical);

    {
        // Création du tableau de bloc
        for(std::size_t i = 0; i < p.m_blocks_count; i++) {
            int2 coords = p.m_blocks_array[i].get_coord();
            int2 insert_coords = coords;
            uint8_t size = p.m_blocks_array[i].get_size();

            for(int j = 0; j < size; j++) {
                blocs_map[insert_coords.x + insert_coords.y * 6] = 1;
                if(p.m_blocks_array[i].get_orientation() == Orientation::horizontal)
                    insert_coords.x++;
                else
                    insert_coords.y++;
            }
        }

        // Test de is_cell_empty
        for(int j = 0; j < 6; j++) 
            for(int i = 0; i < 6; i++) {
                test_map[i + j * 6] = p.is_cell_empty(int2{i, j});
                if(test_map[i + j * 6] == blocs_map[i + j * 6]) {
                    nb_erreur++;
                    std::cout << "is_cell_empty(" << (int2){i, j} << ") renvoie " << test_map[i + j * 6] << "\n";
                }
                
            }

        // Test de can_block_move
        if(!p.can_block_move(p.m_blocks_array[0], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 1 vers la gauche\n"; 
        }
        if(!p.can_block_move(p.m_blocks_array[0], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 1 vers la droite\n"; 
        }
        if(p.can_block_move(p.m_blocks_array[0], 2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 ne devrait pas pouvoir se déplacer de 2 vers la droite\n"; 
        }
        
        if(!p.can_block_move(p.m_blocks_array[1], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le bas\n";
        }
        if(!p.can_block_move(p.m_blocks_array[1], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le haut\n";
        }
        if(!p.can_block_move(p.m_blocks_array[1], -2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 2 vers le haut\n";
        }

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "is_cell_empty:      Données réelles:\n";
            for(int j = 0; j < 6; j++) {
                std::cout << " ";
                for(int i = 0; i < 6; i++)
                    std::cout << test_map[i + j * 6] << " ";
                std::cout << "        ";
                for(int i = 0; i < 6; i++)
                    std::cout << blocs_map[i + j * 6] << " ";
                std::cout << "\n";
            }
        }
    }
    p.move_block(p.m_blocks_array[0], 1);
    //p.move_block(p.m_blocks_array[1], -1);
    {
        // Création du tableau de bloc
        for(std::size_t i = 0; i < p.m_blocks_count; i++) {
            int2 coords = p.m_blocks_array[i].get_coord();
            int2 insert_coords = coords;
            uint8_t size = p.m_blocks_array[i].get_size();

            for(int j = 0; j < size; j++) {
                blocs_map[insert_coords.x + insert_coords.y * 6] = 1;
                if(p.m_blocks_array[i].get_orientation() == Orientation::horizontal)
                    insert_coords.x++;
                else
                    insert_coords.y++;
            }
        }

        // Test de is_cell_empty
        for(int j = 0; j < 6; j++) 
            for(int i = 0; i < 6; i++) {
                test_map[i + j * 6] = p.is_cell_empty(int2{i, j});
                if(test_map[i + j * 6] == blocs_map[i + j * 6]) {
                    nb_erreur++;
                    std::cout << "is_cell_empty(" << (int2){i, j} << ") renvoie " << test_map[i + j * 6] << "\n";
                }
                
            }

        // Test de can_block_move
        if(!p.can_block_move(p.m_blocks_array[0], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 1 vers la gauche\n"; 
        }
        if(!p.can_block_move(p.m_blocks_array[0], -2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 2 vers la gauche\n"; 
        }
        if(p.can_block_move(p.m_blocks_array[0], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 ne devrait pas pouvoir se déplacer de 1 vers la droite\n"; 
        }
        
        if(!p.can_block_move(p.m_blocks_array[1], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le bas\n";
        }
        if(!p.can_block_move(p.m_blocks_array[1], 2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 2 vers le haut\n";
        }
        if(!p.can_block_move(p.m_blocks_array[1], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le haut\n";
        }

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "is_cell_empty:      Données réelles:\n";
            for(int j = 0; j < 6; j++) {
                std::cout << " ";
                for(int i = 0; i < 6; i++)
                    std::cout << test_map[i + j * 6] << " ";
                std::cout << "        ";
                for(int i = 0; i < 6; i++)
                    std::cout << blocs_map[i + j * 6] << " ";
                std::cout << "\n";
            }
        }
    }
    
    if(nb_erreur != 0)
        return false;
    return true;
}