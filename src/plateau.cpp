#include "plateau.hpp"
#include "bloc.hpp"

bool Plateau::m_collision_array[36]{0};
Plateau* Plateau::m_loaded_plateau = nullptr;

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
        m_collision_array[i] = false;
}

void Plateau::load() {
    m_loaded_plateau = this;
    clear_collision_array();

    for(std::size_t i = 0 ; i < m_blocks_count; i++) {
        int2 coords = m_blocks_array[i].get_coord();

        for(int j = 0; j < m_blocks_array[i].get_size(); j++) {
            m_collision_array[coords.x + coords.y * 6] = true;
            
            if(m_blocks_array[i].get_orientation() == Orientation::horizontal)
                coords.x++;
            else
                coords.y++;
        }
    }
}

bool Plateau::can_block_move(const Bloc& block, int displacement) {
    int2 coords = block.get_coord();

    int offset = displacement + (displacement > 0 ? block.get_size() - 1 : 0);
    if(block.get_orientation() == Orientation::horizontal)
        coords.x += offset;
    else
        coords.y += offset;

    return !m_collision_array[coords.x + coords.y * 6];
}

std::unique_ptr<Plateau> Plateau::move_block(Bloc& block, int displacement) {
    std::vector<Bloc> res;
    int2 new_coord = block.get_coord();
    int2 prev_coord = block.get_coord();
    
    if(block.get_orientation() == Orientation::horizontal)
        new_coord.x += displacement;
    else
        new_coord.y += displacement;
    
    // Déplace le block
    block.set_coord(new_coord);
    for(std::size_t i = 0; i < m_loaded_plateau->m_blocks_count; i++) {
        res.push_back(m_loaded_plateau->m_blocks_array[i]);
    }
    // Puis le remet dans l'état inital
    block.set_coord(prev_coord);

    return make_unique<Plateau>(res);
}

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
            if(blocs_map[i] != Plateau::m_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
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
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
        }
    }
    
    std::cout << "Premier test réussi" << std::endl;

    std::unique_ptr<Plateau> p2 = p.move_block(p.m_blocks_array[0], 1);

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
            if(blocs_map[i] != Plateau::m_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
        }

        // Test de can_block_move
        if(!p2->can_block_move(p2->m_blocks_array[0], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 1 vers la gauche\n"; 
        }
        if(!p2->can_block_move(p2->m_blocks_array[0], -2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 2 vers la gauche\n"; 
        }
        if(p2->can_block_move(p2->m_blocks_array[0], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 ne devrait pas pouvoir se déplacer de 1 vers la droite\n"; 
        }
        
        if(!p2->can_block_move(p2->m_blocks_array[1], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le bas\n";
        }
        if(!p2->can_block_move(p2->m_blocks_array[1], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le haut\n";
        }
        if(!p2->can_block_move(p2->m_blocks_array[1], -2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 2 vers le haut\n";
        }

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
        }
    }
    
    std::cout << "Deuxième test réussi\n";
    // On fait remonter le bloc 1
    std::unique_ptr<Plateau> p3 = p2->move_block(p2->m_blocks_array[1], -1);


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
            if(blocs_map[i] != Plateau::m_collision_array[i]) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
        }

        // Test de can_block_move
        if(!p3->can_block_move(p3->m_blocks_array[0], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 1 vers la gauche\n"; 
        }
        if(!p3->can_block_move(p3->m_blocks_array[0], -2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 devrait pouvoir se déplacer de 2 vers la gauche\n"; 
        }
        if(p3->can_block_move(p3->m_blocks_array[0], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 0 ne devrait pas pouvoir se déplacer de 1 vers la droite\n"; 
        }
        
        if(!p3->can_block_move(p3->m_blocks_array[1], 1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le bas\n";
        }
        if(!p3->can_block_move(p3->m_blocks_array[1], 2)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 2 vers le bas\n";
        }
        if(!p3->can_block_move(p3->m_blocks_array[1], -1)) {
            nb_erreur++;
            std::cout << "Le bloc d'index 1 devrait pouvoir se déplacer de 1 vers le haut\n";
        }

        // Affichage en cas d'erreur
        if(nb_erreur != 0) {
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, Plateau::m_collision_array);
        }
    }

    if(nb_erreur != 0)
        return false;
    return true;

}