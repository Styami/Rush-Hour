#include "plateau.hpp"
#include "bloc.hpp"

bool Plateau::s_collision_array[36]{0};
Plateau* Plateau::s_loaded_plateau = nullptr;

Plateau::Plateau(std::size_t nb_blocks) :
    m_blocks_array(new Bloc[nb_blocks]), m_blocks_count(nb_blocks)
{}

Plateau::Plateau(std::vector<Bloc> blocks) :
    m_blocks_array(new Bloc[blocks.size()]), m_blocks_count(blocks.size())
{
    int i = 0;
    for(Bloc b : blocks)
        m_blocks_array[i++].set_data(b.get_raw());
}

Plateau::~Plateau() {
    delete [] m_blocks_array;
}

void Plateau::clear_collision_array() {
    for(int i = 0 ; i < 36; i++)
        s_collision_array[i] = false;
}

void Plateau::load() {
    s_loaded_plateau = this;
    clear_collision_array();

    for(std::size_t i = 0 ; i < m_blocks_count; i++) {
        int2 coords = m_blocks_array[i].get_coord();

        for(int j = 0; j < m_blocks_array[i].get_size(); j++) {
            s_collision_array[coords.x + coords.y * 6] = true;
            
            if(m_blocks_array[i].get_orientation() == Orientation::horizontal)
                coords.x++;
            else
                coords.y++;
        }
    }
}

// Algorithme:
// On va itérer sur tous les blocs du plateau.
// Si on peut déplacer le bloc, alors on crée un nouveau plateau qui devient
// voisin de la configuration actuelle.
// Si on ne peut plus déplacer le bloc, on recommence dans la direction opposé
std::vector<std::unique_ptr<Plateau>> Plateau::get_neighbours() {
    load();

    std::vector<std::unique_ptr<Plateau>> res;
    int displacement;

    for(std::size_t block_i = 0; block_i < m_blocks_count; block_i++) {
        displacement = 1;
        while(can_block_move(block_i, displacement)) {
            res.push_back(move_block(block_i, displacement));
            displacement++;
        }
        displacement = -1;
        while(can_block_move(block_i, displacement)) {
            res.push_back(move_block(block_i, displacement));
            displacement--;
        }
    }
    
    return res;
}

bool Plateau::can_block_move(int block_index, int displacement) {
    Bloc& b = s_loaded_plateau->m_blocks_array[block_index];
    int2 coords = b.get_coord();

    int offset = displacement + (displacement > 0 ? b.get_size() - 1 : 0);
    if(b.get_orientation() == Orientation::horizontal)
        coords.x += offset;
    else
        coords.y += offset;

    if(coords.x < 0 || coords.y < 0 || coords.x > 5 || coords.y > 5)
        return false;

    return !s_collision_array[coords.x + coords.y * 6];
}

std::unique_ptr<Plateau> Plateau::move_block(int block_index, int displacement) {
    std::vector<Bloc> res;
    Bloc& b = s_loaded_plateau->m_blocks_array[block_index];
    int2 new_coord = b.get_coord();
    
    if(b.get_orientation() == Orientation::horizontal)
        new_coord.x += displacement;
    else
        new_coord.y += displacement;
    
    for(std::size_t i = 0; i < s_loaded_plateau->m_blocks_count; i++) {
        res.push_back(s_loaded_plateau->m_blocks_array[i]);
    }
    res[block_index].set_coord(new_coord);

    return make_unique<Plateau>(res);
}


//
//
//     TESTS UNITAIRES
//
//

void static load_blocs_map(bool* arr, const Bloc* p, std::size_t p_size) {
    for(int i = 0; i < 36; i++)
        arr[i] = false;
    for(std::size_t i = 0; i < p_size; i++) {
        int2 coords = p[i].get_coord();
        int2 insert_coords = coords;
        uint8_t size = p[i].get_size();

        for(int j = 0; j < size; j++) {
            arr[insert_coords.x + insert_coords.y * 6] = 1;
            if(p[i].get_orientation() == Orientation::horizontal)
                insert_coords.x++;
            else
                insert_coords.y++;
        }
    }
}

void static print_debug_tab(bool* blocs_map, bool* collision_array) {
    for(int j = 0; j < 6; j++) {
        std::cout << " ";
        for(int i = 0; i < 6; i++)
            std::cout << blocs_map[i + j * 6] << " ";
        std::cout << "        ";
        for(int i = 0; i < 6; i++)
            std::cout << collision_array[i + j * 6] << " ";
        std::cout << "\n";
    }
}

void Plateau::test_can_block_move(const Plateau& p, int index, int displacement, bool expected_result, int& nb_erreur) {
    if(p.can_block_move(index, displacement) != expected_result) {
        nb_erreur++;
        std::cout << "Le bloc d'index " << index << 
            (expected_result == false ? "ne" : "") <<
            " devrait " <<
            (expected_result == false ? "pas" : "") <<
            " pouvoir se déplacer de " << abs(displacement) << " vers ";
        if(p.m_blocks_array[index].get_orientation() == Orientation::horizontal)
            std::cout << (displacement < 0 ? "la gauche" : "la droite");
        else
            std::cout << (displacement < 0 ? "le haut" : "le bas");
        std::cout << "\n";
    }
}

bool Plateau::test() {
    int nb_erreur = 0;
    bool last_failed = false;

    Plateau p(2);
    bool blocs_map[36]{0};

    p.m_blocks_array[0].set_data(1, 2, false, Orientation::horizontal);
    p.m_blocks_array[1].set_data(4, 2, true, Orientation::vertical);

    {
        // Création du tableau de bloc
        load_blocs_map(blocs_map, p.m_blocks_array, p.m_blocks_count);

        // Test load
        p.load();

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != Plateau::s_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }

        // Test de can_block_move
        test_can_block_move(p, 0, -1, true, nb_erreur);
        test_can_block_move(p, 0, 1, true, nb_erreur);
        test_can_block_move(p, 0, 2, false, nb_erreur);
        test_can_block_move(p, 1, 1, true, nb_erreur);
        test_can_block_move(p, 1, -1, true, nb_erreur);
        test_can_block_move(p, 1, -2, true, nb_erreur);

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }
    }

    std::unique_ptr<Plateau> p2 = p.move_block(0, 1);

    // Test move block
    if(p2->m_blocks_array[0].get_raw() != p.m_blocks_array[0].get_raw() + 0b00100000) {
        std::cout << "Le premier bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }
    if(p2->m_blocks_array[1].get_raw() != p.m_blocks_array[1].get_raw()) {
        std::cout << "Le deuxième bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }
 
    {
        // Création du tableau de bloc
        load_blocs_map(blocs_map, p2->m_blocks_array, p2->m_blocks_count);

        // Test load
        p2->load();

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != Plateau::s_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }

        // Test de can_block_move
        test_can_block_move(*p2, 0, -1, true, nb_erreur);
        test_can_block_move(*p2, 0, -2, true, nb_erreur);
        test_can_block_move(*p2, 0, 1, false, nb_erreur);
        test_can_block_move(*p2, 1, 1, true, nb_erreur);
        test_can_block_move(*p2, 1, -1, true, nb_erreur);
        test_can_block_move(*p2, 1, -2, true, nb_erreur);
        
        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }
    }
    
    // On fait remonter le bloc 1
    std::unique_ptr<Plateau> p3 = p2->move_block(1, -1);

    // Test move block
    if(p3->m_blocks_array[0].get_raw() != p2->m_blocks_array[0].get_raw()) {
        std::cout << "Le premier bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }
    if(p3->m_blocks_array[1].get_raw() != p2->m_blocks_array[1].get_raw() - 0b00000100) {
        std::cout << "Le deuxième bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }

    {
        // Création du tableau de bloc
        load_blocs_map(blocs_map, p3->m_blocks_array, p3->m_blocks_count);

        // Test load
        p3->load();

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != Plateau::s_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }

        // Test de can_block_move
        test_can_block_move(*p3, 0, -1, true, nb_erreur);
        test_can_block_move(*p3, 0, -2, true, nb_erreur);
        test_can_block_move(*p3, 0, 1, false, nb_erreur);
        test_can_block_move(*p3, 1, 1, true, nb_erreur);
        test_can_block_move(*p3, 1, 2, true, nb_erreur);
        test_can_block_move(*p3, 1, -1, true, nb_erreur);

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::s_collision_array);
        }
    }

    if(nb_erreur != 0)
        return false;
    return true;

}