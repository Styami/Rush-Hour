#ifndef __PLATEAU__
#define __PLATEAU__

#include "bloc.hpp"

#include <memory>
#include <string>
#include <iostream>
#include <vector>

/**
 * @brief Chaque instance de plateau devra stocker les blocs qui le composent, ainsi que le bloc à sortir et les coordonnées de la sorties.
 * Les blocs ont déjà une bonne complexité spatial, mais pour tester les collisions il faudrait itérer sur chacun d'entre eux.
 * On va donc ajouter un tableau de collision de manière statique (pour ne pas peser plus lourd en mémoire) et simplifier les calculs de collisions.
 * Une instance de plateau pourra donc être "chargée" (il deviendra le plateau que l'on traite): 
 *      On itèrera une seule fois sur tous les blocs du plateau pour construire le tableau de collision.
 *      On pourra tester si un bloc est déplaçable.
 *      On pourra déplacer un bloc et obtenir une nouvelle instance de plateau.
 */
class Plateau {
public:
    Plateau();

    /**
     * @brief Construit un plateau à partir des blocs passé en paramètre.
     * 
     * @param blocks 
     */
    Plateau(std::vector<Bloc> blocks);

    /**
     * @brief Construit un plateau en déplacant les données de celui passé en paramètre
     * 
     * @param p 
     */
    Plateau(Plateau&& p);

    Plateau(const Plateau& p);

    /**
     * @brief Construit un plateau à partir d'un fichier
     * 
     * @param file_path 
     */
    Plateau(const std::string& file_path);

    /**
     * @brief Opérateur d'assignement par déplacement
     * 
     * @param p 
     * @return Plateau 
     */
    Plateau& operator=(Plateau&& p);

    Plateau & operator=(const Plateau & p);

    ~Plateau();

    /**
     * @brief Renvoie une liste de toutes les configurations de plateau accessible depuis celui ci.
     * 
     * @return std::vector<std::unique_ptr<Plateau>> Liste de configuration de plateau 
     */
    std::vector<std::unique_ptr<Plateau>> get_neighbours();

    /**
     * @brief Génère un plateau aléatoirement avec le nombre de bloc passé en paramètre
     * 
     * @param nb_block Nombre de bloc du plateau
     * @return true La génération a réussi
     * @return false La génération a échouée
     */
    bool generer_aleatoirement(int nb_block);

    /**
     * @brief Charge le plateau depuis un fichier
     * 
     * @param file_path Chemin vers le fichier
     */
    void charger(std::string file_path);

    /**
     * @brief Sauvegarde le plateau dans un fichier
     * 
     * @param file_path Chemin vers le fichier
     */
    void sauvegarder(std::string file_path);

    /**
     * @brief Renvoie vrai si le plateau actuel est gagnant
     * 
     */
    bool est_gagnant() const;

    /**
     * @brief Renvoie le tableau des blocs pour pouvoir les afficher
     * 
     * @return const Bloc* 
     */
    const Bloc* get_block_array() const { return m_blocks_array; };
    
    /**
     * @brief Renvoie le nombre de bloc dans le plateau
     *  Lit cette information depuis s_plateau_data
     * @return std::size_t 
     */                                                                        // 0x000000f000000000 correspond aux bits 36 à 39
    static std::size_t get_block_count() { return (std::size_t)((s_plateau_data & 0x000000f000000000) >> 36) + 1; };

    /**
     * @brief Renvoie l'index du bloc à sortir
     *  Lit cette information depuis s_plateau_data
     * @return std::size_t index du bloc gagnant
     */// 0x00000f0000000000 correspond aux bits 40 à 43
    static std::size_t get_winning_block() { return (std::size_t)((s_plateau_data & 0x00000f0000000000) >> 40); };

    // 
    //     Gestion table de hashage
    // 

    /**
     * @brief Opérateur d'égalité utilisé pour la hashmap
     * 
     * @param p 
     * @return true 
     * @return false 
     */
    bool operator==(const Plateau& p) const;

    /**
     * @brief Fonction de hashage utilisée pour la hashmap
     * 
     * @return std::size_t 
     */
    std::size_t hash() const;

    struct Thash {
        std::size_t operator() (Plateau* p) const { return p->hash();}
    };

    struct Tequal {
        bool operator() (Plateau* a, Plateau* b) const { return *a == *b; }
    };


    //     
    //      Test unitaires
    // 

    static bool test();

private:

    Bloc* m_blocks_array;

    /**
     * @brief Condense les informations relatives aux plateaux de jeu
     * Le nombre de bloc total (1 minimum et 16 maximum)
     * L'index du bloc à faire sortir (compris entre 0 et 15)
     * Le tableau de collision pour faire les tests plus rapidement
     *
     * La porte de sortie sera déduite de la position du bloc à faire sortir
     *      puisqu'elle sera forcément sur le bord droit.
     *
     * 0-35 : Tableau de collision
     * 36-39 : Nombre de blocs
     * 40-43 : Index du bloc à sortir
     */
    static uint64_t s_plateau_data;

    /**
     * @brief Pointeur sur le plateau chargé
     */
    static Plateau* s_loaded_plateau;

    /**
     * @brief Constructeur pour les tests, ne sera pas utilisé ailleurs
     * @param nb_blocks Nombre de blocs à créer
     */
    Plateau(std::size_t nb_blocks);

    /**
     * @brief Défini le plateau comme actif vis à vis des tests de collisions et récupération des voisins.
     */
    void load();

    /**
     * @brief Vérifie si un bloc peut se déplacer d'une certaine distance
     * Suppose que les déplacements antérieurs sont possible.
     *   Ne vérifie que si l'extrémité du bloc arrive sur une case vide.
     * @param block Une référence sur un bloc
     * @param displacement La distance à laquelle se déplacer
     *      <0 : Coin supérieur gauche
     *      >0 : Coin inférieur droit
     * @return true Le bloc peut se déplacer
     * @return false Le bloc ne peut pas se déplacer
     */
    static bool can_block_move(int block_index, int displacement);

    /**
     * @brief Renvoie un pointeur sur plateau dans la nouvelle configuration (après déplacement)
     * La durée de vie du pointeur doit être gérer par le graph
     * 
     * @param block_index Une référence sur un bloc
     * @param displacement La distance à laquelle se déplacer
     *      <0 : Coin supérieur gauche
     *      >0 : Coin inférieur droit
     * @return std::weak_ptr<Plateau> 
     */
    static std::unique_ptr<Plateau> move_block(int block_index, int displacement);

    /**
     * @brief Ajoute une collision sur la case désignée.
     *  Écrit cette information sur s_plateau_data
     * @param pos Coordonnée de la case
     */
    static void add_collision(const Bloc& block);

    /**
     * @brief Test si la case est occupé par un bloc
     *  Lit cette information depuis s_plateau_data
     * @param pos Coordonnée de la case a tester
     */
    static bool test_collision(uint2 pos);
    static bool test_collision(int offset);

    /**
     * @brief Test si un bloc peut rentrer à l'emplacement indiqué
     * 
     * @param pos Position du bloc
     * @param size Taille du bloc
     * @param orientation Orientation du bloc
     */
    bool test_can_block_fit(uint2 pos, bool size, Orientation orientation);

    /**
     * @brief Stock le nombre de bloc du plateau
     *  Écrit cette information sur s_plateau_data
     * @param count Nombre de bloc
     */
    static void set_block_count(uint64_t count);

    /**
     * @brief Stock l'index du bloc à sortir
     *  Écrit cette information sur s_plateau_data
     * @param index Index du bloc à sortir
     */
    static void set_winning_block(uint64_t index);

    


    //     
    //      Test unitaires
    // 

    static void test_lecture_ecriture(int& nb_erreur);
    static bool test_can_block_move(const Plateau& p, int index, int displacement, bool expected_result, int& nb_erreur);
    static void get_collision_array(bool* array);
};

#endif