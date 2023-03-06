#ifndef __PLATEAU__
#define __PLATEAU__

#include "bloc.hpp"
#include <memory>
#include <string>
#include <iostream>


class Plateau {
public:
    /**
     * @brief Constructeur pour les tests, ne sera pas utilisé dans la version finale
     * @param nb_blocks Nombre de blocs à créer
     */
    Plateau(std::size_t nb_blocks);

    /**
     * @brief Constructeur du plateau. Lit un fichier et instancie le bon nombre de blocs
     * et les variables membres.
     * 
     * @param file_path Chemin vers le fichier de jeu
     */
    Plateau(std::string file_path);

    ~Plateau();

    /**
     * @brief Vérifie si un bloc peut se déplacer d'une certaine distance
     * Suppose que les déplacements antérieurs sont possible.
     *   Ne vérifie que si l'extrémité du bloc arrive sur une case vide.
     *   Ne vérifie pas si le déplacement envoie le bloc en dehors de la map
     * @param block Une référence sur un bloc
     * @param displacement La distance à laquelle se déplacer
     *      <0 : Coin supérieur gauche
     *      >0 : Coin inférieur droit
     * @return true Le bloc peut se déplacer
     * @return false Le bloc ne peut pas se déplacer
     */
    bool can_block_move(const Bloc& block, int displacement) const;

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
    std::unique_ptr<Plateau> move_block(const Bloc& block, int displacement) const;

    static bool test();

private:
    /** @brief Tableau de bloc, à tenir à jour après chaque màj du plateau de jeu
     *   On l'utilisera pour récupérer directement la version condensée du plateau
     *   de jeu, pour réduire le temps d'exécution lors de la génération des noeuds
     * du graph
     */
    Bloc* m_blocks_array;

    /**
     * @brief Stock le nombre de blocs contenu dans le plateau de jeu
     */
    std::size_t m_blocks_count;

    /**
     * @brief Vérifie si la cellule est occupé par un bloc
     * 
     * @param coords Coordonnée testée
     * @return true La cellule contient un bloc
     * @return false La cellule ne contient pas de bloc
     */
    bool is_cell_empty(int2 coords) const;

    /// Charge un fichier en mémoire. Renvoie un tableau de blocs
    static Bloc* load_file(std::string file_path);

    /// Renvoie le nombre de blocs du plateau en lisant depuis un fichier
    /// nécessaire pour instancier m_blocs_count
    static std::size_t get_block_count_from_file(std::string file_path);
};

#endif