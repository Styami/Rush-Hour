#include "plateau.hpp"

#include "bloc.hpp"
#include "utils.hpp"

#include <fstream>

uint64_t Plateau::s_plateau_data = 0;
Plateau* Plateau::s_loaded_plateau = nullptr;

Plateau::Plateau() :
    m_blocks_array(nullptr)
{}

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
        m_blocks_array[i++] = b.get_raw();
}

Plateau::Plateau(Plateau&& p) :
    m_blocks_array(std::move(p.m_blocks_array))
{
    p.m_blocks_array = nullptr;
}

Plateau::Plateau(const Plateau& p)
{
    m_blocks_array = new Bloc[get_block_count()];
    for(std::size_t i = 0; i < get_block_count(); i++)
    {
        m_blocks_array[i] = p.m_blocks_array[i];
    }
}

Plateau::Plateau(const std::string& file_path) :
    m_blocks_array(nullptr)
{
    charger(file_path);
}

Plateau& Plateau::operator=(Plateau&& p) {
    if(&p != this) {
        delete [] m_blocks_array;

        m_blocks_array = std::move(p.m_blocks_array);

        p.m_blocks_array = nullptr;
    }
    return *this;
}

Plateau & Plateau::operator=(const Plateau & p) 
{
    m_blocks_array = p.m_blocks_array;
    return *this;
}

Plateau::~Plateau() {
    delete [] m_blocks_array;
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


static void melanger(uint2* array, std::size_t count)
{
    uint2 swap;
    int cur_count = count;
    for(std::size_t i = 0; i < count - 1; i++)
    {
        int index = rand() % cur_count;
        swap = array[cur_count - 1];
        array[cur_count - 1] = array[index];
        array[index] = swap;

        cur_count--;
    }
}

// Les commentaires expliquent plutôt bien le pseudo code
bool Plateau::generer_aleatoirement(int nb_block)
{
// Allocation mémoire
    unsigned int start_bloc_y;

    if(m_blocks_array != nullptr)
        delete [] m_blocks_array;
    m_blocks_array = new Bloc[nb_block];
    
// On reset le tableau statique
    s_plateau_data = 0;
    set_block_count(nb_block);
    set_winning_block(0);

// On génère d'abord le bloc à sortir
    start_bloc_y = rand() % 6;

    m_blocks_array[0].set_data(0, start_bloc_y, rand() % 2, Orientation::horizontal);
    add_collision(m_blocks_array[0]);

// On génère une liste de toutes les positions où l'on peut ajouter un bloc
// Sauf l'angle inférieur droit qui ne peut pas accueillir de nouveau bloc
    uint2 random_pos[35];
    for(int i = 0; i < 35; i++)
    {
        random_pos[i] = uint2(i % 6, i / 6.f);
    }
    melanger(random_pos, 35);

// On crée une liste des différents arrangement possible pour un bloc.
// (size, orientation)
    uint2 arrangement[4];
    arrangement[0] = {0, 0};
    arrangement[1] = {0, 1};
    arrangement[2] = {1, 0};
    arrangement[3] = {1, 1};

// On génère nb_block - 1 blocs
    int cur_gen = 1, i = 0;
    while(cur_gen < nb_block && i < 35)
    {
        // On test aléatoirement tous les arrangements possibles jusqu'à ce que ça fonctionne
        // Si jamais aucun ne fonctionne, alors cette position peut être rayée de la liste des
        //  possibilités
        melanger(arrangement, 4);
        for(int j = 0; j < 4; j++)
        {
            // On vérifie que le bloc qu'on ajoute n'empêche pas de sortir le bloc
            if(random_pos[i].y == start_bloc_y && arrangement[j].y == 0)
                continue;

            // Si le bloc peut rentrer
            if(test_can_block_fit(random_pos[i],
                arrangement[j].x,
                arrangement[j].y ? Orientation::vertical : Orientation::horizontal
            )) {
                // On l'ajoute au tableau
                m_blocks_array[cur_gen].set_data(
                    random_pos[i].x,
                    random_pos[i].y,
                    arrangement[j].x,
                    arrangement[j].y ? Orientation::vertical : Orientation::horizontal
                );
                // Et à la table de collision
                add_collision(m_blocks_array[cur_gen]);
                cur_gen++;
                break;
            }
        }
        i++;
    }
    if(cur_gen < nb_block)
        return false;
    return true;
}

void Plateau::charger(std::string file_path) 
{
    std::ifstream file(file_path);
    assert(file.is_open());
    unsigned char read;

    // Nombre de block
    read = file.get();
    set_block_count(read);

    // Allocation mémoire du tableau
    if(m_blocks_array != nullptr)
        delete [] m_blocks_array;
    m_blocks_array = new Bloc[get_block_count()];

    // Bloc gagnant
    read = file.get();
    set_winning_block(read);

    // Création des blocs
    for(std::size_t i = 0; i < get_block_count(); i++) {
        read = file.get();
        m_blocks_array[i] = read;
    }
    file.close();
}

void Plateau::sauvegarder(std::string file_path) 
{
    std::ofstream file(file_path);

    file << (unsigned char)get_block_count();
    file << (unsigned char)get_winning_block();

    for(std::size_t i = 0 ; i < get_block_count(); i++) {
        file << m_blocks_array[i].get_raw();
    }

    file.close();
}

bool Plateau::est_gagnant() const{
    const Bloc& winning_bloc = m_blocks_array[get_winning_block()];
    if(winning_bloc.get_coord().x + winning_bloc.get_size() >= 6)
        return true;
    return false;
}

void Plateau::load() {
    s_loaded_plateau = this;
    s_plateau_data &= 0xfffffff000000000;

    for(std::size_t i = 0 ; i < get_block_count(); i++) {
        add_collision(m_blocks_array[i]);
    }
}

bool Plateau::can_block_move(int block_index, int displacement) {
    Bloc& b = s_loaded_plateau->m_blocks_array[block_index];
    uint2 coords = b.get_coord();

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
    uint2 new_coord = b.get_coord();
    
    if(b.get_orientation() == Orientation::horizontal)
        new_coord.x += displacement;
    else
        new_coord.y += displacement;
    
    for(std::size_t i = 0; i < get_block_count(); i++) {
        res.push_back(s_loaded_plateau->m_blocks_array[i]);
    }
    res[block_index].set_coord(new_coord);

    return std::make_unique<Plateau>(res);
}

void Plateau::add_collision(const Bloc& block) {
    uint2 coords = block.get_coord();

    for(int j = 0; j < block.get_size(); j++) {
        uint64_t encoded_pos = 1;
        encoded_pos <<= coords.y * 6;
        encoded_pos <<= coords.x;
        s_plateau_data |= encoded_pos;

        if(block.get_orientation() == Orientation::horizontal)
            coords.x++;
        else
            coords.y++;
    }
}

bool Plateau::test_collision(uint2 pos) {
    uint64_t encoded_pos = 1;
    encoded_pos <<= pos.y * 6;
    encoded_pos <<= pos.x;
    return (bool)(encoded_pos & s_plateau_data);
}

bool Plateau::test_collision(int offset) {
    uint64_t encoded_pos = 1;
    encoded_pos <<= offset;
    return (bool)(encoded_pos & s_plateau_data); 
}

bool Plateau::test_can_block_fit(uint2 pos, bool size, Orientation orientation)
{
    for(int j = 0 ; j < (size ? 3 : 2); j++ ) {
        if(test_collision(pos) || pos.x > 5 || pos.y > 5) {
            return false;
        }
        if(orientation == Orientation::horizontal)
            pos.x++;
        else
            pos.y++;
    }
    return true;
}

void Plateau::set_block_count(uint64_t count) {
    assert(count > 0 && count <= 16);
    s_plateau_data &= s_plateau_data & 0xffffff0fffffffff; 
    s_plateau_data |= ((count - 1) << 36);
}


void Plateau::set_winning_block(uint64_t index) {
    assert(index >= 0 && index < 16);
    s_plateau_data &= 0xfffff0ffffffffff;
    s_plateau_data |= (index << 40);
}


// 
//     Gestion table de hashage
// 


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


//
//
//     TESTS UNITAIRES
//
//


void static load_blocs_map(bool* arr, const Bloc* p, std::size_t p_size) {
    for(int i = 0; i < 36; i++)
        arr[i] = false;
    for(std::size_t i = 0; i < p_size; i++) {
        uint2 coords = p[i].get_coord();
        uint2 insert_coords = coords;
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

bool Plateau::test_can_block_move(const Plateau& p, int index, int displacement, bool expected_result, int& nb_erreur) {
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
        return true;
    }
    return false;
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

void Plateau::test_lecture_ecriture(int& nb_erreur) {
// Test chargement
    std::ifstream file("data/test/test_data_human_readable.rh");
    unsigned int block_count, winning, x, y, size, orientation;

    file >> block_count;
    Plateau p_init((unsigned int)block_count);

    file >> winning;
    p_init.set_winning_block(winning);
    for(std::size_t i = 0; i < (std::size_t)block_count; i++)
    {
        file >> x >> y >> size >> orientation;
        (p_init.m_blocks_array[i]).set_data(
            x, y,
            size,
            (orientation ? Orientation::vertical : Orientation::horizontal)
        );
    }

    p_init.sauvegarder("data/test/test_data_save.rh");

    file.clear();
    file.seekg(0);
    
    Plateau p_load("data/test/test_data_save.rh");

    file >> block_count;
    if(get_block_count() != block_count) {
        std::cout << "Lecture fichier: Block Count erronné: " << get_block_count() << " au lieu de " << block_count << "\n";
        nb_erreur++;
    }

    file >> winning;
    if(get_winning_block() != winning) {
        std::cout << "Lecture fichier: Winning block erronné: " << get_winning_block() << " au lieu de " << winning << "\n";
        nb_erreur++;
    }
    
    for(std::size_t i = 0; i < block_count; i++) {
        file >> x >> y >> size >> orientation;
        size = (size ? 3 : 2);

        if(p_load.m_blocks_array[i].get_coord().x != x) {
            std::cout << "Lecture fichier bloc " << i << ": Coordonnée x erronnées: " << p_load.m_blocks_array[i].get_coord().x << " au lieu de " << x << "\n";
            nb_erreur++;
        }
        if(p_load.m_blocks_array[i].get_coord().y != y) {
            std::cout << "Lecture fichier bloc " << i << ": Coordonnée y erronnées: " << p_load.m_blocks_array[i].get_coord().y << " au lieu de " << y << "\n";
            nb_erreur++;
        }
        if(p_load.m_blocks_array[i].get_size() != size) {
            std::cout << "Lecture fichier bloc " << i << ": Taille erronnée: " << (int) p_load.m_blocks_array[i].get_size() << " au lieu de " << size << "\n";
            nb_erreur++;
        }
        if(p_load.m_blocks_array[i].get_orientation() != (orientation ? Orientation::vertical : Orientation::horizontal)) {
            std::cout << "Lecture fichier bloc " << i << ": Orientation erronnée: " 
            << (p_load.m_blocks_array[i].get_orientation() == Orientation::vertical ? "vertical" : "horizontal") 
            << " au lieu de " 
            << (orientation == 1 ? "vertical" : "horizontal") 
            << "\n";
            nb_erreur++;
        }
    }

    file.close();
}

bool Plateau::test() {
    int nb_erreur = 0;
    bool last_failed = false;

    test_lecture_ecriture(nb_erreur);

    Plateau p(2);
    if(get_block_count() != 2) {
        std::cout << "Block count erronné " << get_block_count() << " au lieu de 2.";
        //return false;
    }
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
        last_failed = test_can_block_move(p, 0, -1, true, nb_erreur);
        last_failed = test_can_block_move(p, 0, 1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(p, 0, 2, false, nb_erreur) || last_failed;
        last_failed = test_can_block_move(p, 1, 1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(p, 1, -1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(p, 1, -2, true, nb_erreur) || last_failed;

        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
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
        last_failed = test_can_block_move(*p2, 0, -1, true, nb_erreur);
        last_failed = test_can_block_move(*p2, 0, -2, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p2, 0, 1, false, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p2, 1, 1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p2, 1, -1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p2, 1, -2, true, nb_erreur) || last_failed;
        
        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
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
        last_failed = test_can_block_move(*p3, 0, -1, true, nb_erreur);
        last_failed = test_can_block_move(*p3, 0, -2, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p3, 0, 1, false, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p3, 1, 1, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p3, 1, 2, true, nb_erreur) || last_failed;
        last_failed = test_can_block_move(*p3, 1, -1, true, nb_erreur) || last_failed;

        // Affichage en cas d'erreur
        if(last_failed) {
            last_failed = false;
            std::cout << "Données réelles:      Collision arr:\n";
            print_debug_tab(blocs_map, collision_array);
        }
    }

    if(nb_erreur != 0)
        return false;
    return true;
}