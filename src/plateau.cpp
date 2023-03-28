#include "plateau.hpp"
#include "bloc.hpp"

uint64_t Plateau::s_plateau_data = 0;
Plateau* Plateau::s_loaded_plateau = nullptr;

Plateau::Plateau(std::size_t nb_blocks) :
    m_blocks_array(new Bloc[nb_blocks])
{
    set_block_count(nb_blocks);
}

Plateau::Plateau(std::vector<Bloc> blocks) :
    m_blocks_array(new Bloc[blocks.size()])
{
    Plateau::set_block_count(blocks.size());

    int i = 0;
    for(Bloc b : blocks)
        m_blocks_array[i++].set_data(b.get_raw());
}

Plateau::Plateau(Plateau&& p) :
    m_blocks_array(std::move(p.m_blocks_array))
{
    p.m_blocks_array = nullptr;
}

Plateau& Plateau::operator=(Plateau&& p) {
    if(&p != this) {
        delete [] m_blocks_array;

        m_blocks_array = std::move(p.m_blocks_array);

        p.m_blocks_array = nullptr;
    }
    return *this;
}

Plateau::~Plateau() {
    delete [] m_blocks_array;
}

void Plateau::clear_collision_array() {
    s_plateau_data &= 0xfffffff000000000;
}

void Plateau::add_collision(int2 pos) {
    uint64_t encoded_pos = 1;
    encoded_pos <<= pos.y * 6;
    encoded_pos <<= pos.x;
    s_plateau_data |= encoded_pos;
}

bool Plateau::test_collision(int2 pos) {
    uint64_t encoded_pos = 1;
    encoded_pos <<= pos.y * 6;
    encoded_pos <<= pos.x;
    return (bool)(encoded_pos & s_plateau_data);
}

bool Plateau::test_collision(int index) {
    uint64_t encoded_pos = 1;
    encoded_pos <<= index;
    return (bool)(encoded_pos & s_plateau_data); 
}

void Plateau::set_block_count(uint64_t count) {
    assert(count > 0 && count <= 16);

    s_plateau_data |= ((count - 1) << 36);
}

std::size_t Plateau::get_block_count() {
    // 0x000000f000000000 correspond aux bits 36 à 39
    return (std::size_t)((s_plateau_data & 0x000000f000000000) >> 36) + 1;
}

void Plateau::set_winning_block(uint64_t index) {
    assert(index >= 0 && index < 16);

    s_plateau_data |= (index << 40);
}

std::size_t Plateau::get_winning_block() {
    // 0x00000f0000000000 correspond aux bits 40 à 43
    return (std::size_t)((s_plateau_data & 0x00000f0000000000) >> 40);
}

void Plateau::load() {
    s_loaded_plateau = this;
    clear_collision_array();

    for(std::size_t i = 0 ; i < get_block_count(); i++) {
        int2 coords = m_blocks_array[i].get_coord();

        for(int j = 0; j < m_blocks_array[i].get_size(); j++) {
            add_collision(coords);
            
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

    for(std::size_t block_i = 0; block_i < get_block_count(); block_i++) {
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

bool Plateau::est_gagnant() const{
    const Bloc& winning_bloc = m_blocks_array[get_winning_block()];
    if(winning_bloc.get_coord().x + winning_bloc.get_size() >= 6)
        return true;
    return false;
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

    return !test_collision(coords);
}

std::unique_ptr<Plateau> Plateau::move_block(int block_index, int displacement) {
    std::vector<Bloc> res;
    Bloc& b = s_loaded_plateau->m_blocks_array[block_index];
    int2 new_coord = b.get_coord();
    
    if(b.get_orientation() == Orientation::horizontal)
        new_coord.x += displacement;
    else
        new_coord.y += displacement;
    
    for(std::size_t i = 0; i < get_block_count(); i++) {
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

void Plateau::get_collision_array(bool* array) {
    for(int i = 0; i < 36; i++) {
        array[i] = test_collision(i);
    } 
}

static void print_debug_tab(bool* blocs_map, bool* collision_array) {
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

bool Plateau::operator==(const Plateau& p) const {
    for(std::size_t i = 0; i < get_block_count(); i++) {
        if(m_blocks_array[i].get_raw() != p.m_blocks_array[i].get_raw())
            return false;
    }
    return true;
}

std::size_t Plateau::hash() const {
    // Idée récupérée de Stackoverflow: https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
    //      Solution de Karl von Moor
    // N'ayant aucune connaissance technique en hashage, nous avons décidé de trouver une solution existante pour 
    // éviter des cas d'erreur difficilement débuggable

    std::hash<char> hasher;
    std::size_t new_hash = 0;
    for(std::size_t i = 0; i < get_block_count(); i++) {
        new_hash ^= hasher(m_blocks_array[i].get_raw()) + 0xf2ae2ba4 + (new_hash << 6) + (new_hash >> 2);
    }
    return new_hash;
}

bool Plateau::test() {
    int nb_erreur = 0;
    bool last_failed = false;

    Plateau p(2);
    bool blocs_map[36]{0};
    bool collision_array[36];

    p.m_blocks_array[0].set_data(1, 2, false, Orientation::horizontal);
    p.m_blocks_array[1].set_data(4, 2, true, Orientation::vertical);

    {
        // Création du tableau de bloc
        load_blocs_map(blocs_map, p.m_blocks_array, get_block_count());

        // Test load
        p.load();
        get_collision_array(collision_array);

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != test_collision(i)) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, collision_array);
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
            print_debug_tab(blocs_map, collision_array);
        }
    }

    std::unique_ptr<Plateau> p2 = std::move(p.move_block(0, 1));

    // Test move block
    if(p2->m_blocks_array[0].get_raw() != p.m_blocks_array[0].get_raw() + 0b00100000) {
        std::cout << "Le premier bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }
    if(p2->m_blocks_array[1].get_raw() != p.m_blocks_array[1].get_raw()) {
        std::cout << "Le deuxième bloc dans le nouveau plateau n'a pas les bonnes valeurs\n";
    }
 
    {
        // Création du tableau de bloc
        load_blocs_map(blocs_map, p2->m_blocks_array, get_block_count());

        // Test load
        p2->load();
        get_collision_array(collision_array);

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != test_collision(i)) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, collision_array);
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
            print_debug_tab(blocs_map, collision_array);
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
        load_blocs_map(blocs_map, p3->m_blocks_array, get_block_count());

        // Test load
        p3->load();
        get_collision_array(collision_array);

        for(std::size_t i = 0; i < 36; i++) {
            if(blocs_map[i] != test_collision(i)) {
                nb_erreur++;
                last_failed = true;
            }
        }
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Le tableau de collision ne correspond pas au tableau chargé\n";
            std::cout << "Données réelles:      collision arr:\n";
            print_debug_tab(blocs_map, collision_array);
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
            print_debug_tab(blocs_map, collision_array);
        }
    }

    if(nb_erreur != 0)
        return false;
    return true;

}