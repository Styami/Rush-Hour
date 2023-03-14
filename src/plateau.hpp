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

    /**
     * @brief Opérateur d'assignement par déplacement
     * 
     * @param p 
     * @return Plateau 
     */
    Plateau& operator=(Plateau&& p);

    ~Plateau();

    /**
     * @brief Renvoie une liste de toutes les configurations de plateau accessible depuis celui ci.
     * 
     * @return std::vector<std::unique_ptr<Plateau>> Liste de configuration de plateau 
     */
    std::vector<std::unique_ptr<Plateau>> get_neighbours();


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
        std::size_t operator() (const Plateau& p) const { return p.hash();}
    };

    struct Tequal {
        bool operator() (const Plateau& a, const Plateau& b) const { return a == b; }
    };


    //     
    //      Test unitaires
    // 

    /**
     * @brief Fonction de test
     * 
     * @return true La classe fonctionne comme attendu
     * @return false La classe contient des bugs
     */
    static bool test();

private:
    /**
     * @brief Constructeur pour les tests, ne sera pas utilisé dans la version finale
     * @param nb_blocks Nombre de blocs à créer
     */
    Plateau(std::size_t nb_blocks);

    /** @brief Tableau de blocs composant le plateau de jeu
     */
    Bloc* m_blocks_array;

    /**
     * @brief Stock le nombre de blocs contenu dans le plateau de jeu
     */
    std::size_t m_blocks_count;

    /**
     * @brief Tableau de booléens représentant si la case est occupée par un bloc ou non
     */
    static bool s_collision_array[36];

    /**
     * @brief Pointeur sur le plateau chargé
     * 
     */
    static Plateau* s_loaded_plateau;
    
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

    static void clear_collision_array();

    static void test_can_block_move(const Plateau& p, int index, int displacement, bool expected_result, int& nb_erreur);
};

#endif