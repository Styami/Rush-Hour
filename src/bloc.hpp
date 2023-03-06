#ifndef __BLOC__
#define __BLOC__

#include "utils.hpp"
#include <assert.h>
#include <iostream>

enum class Orientation {
    vertical,
    horizontal
};

class Bloc {
public:
    // Crée un bloc dans l'espace mémoire
    // initalise toutes les valeurs à 0
    Bloc();

    // Crée un bloc dans l'espace mémoire
    // _x € [0; 5]
    // _y € [0; 5]
    // _size: false : taille 2    true : taille 3
    Bloc(char _x, char _y, bool _size, Orientation _orientation);

    /**
     * @brief Constructeur par copie
     * 
     * @param b bloc original
     */
    Bloc(Bloc& b);

    // Renvoie un int2 contenant la position x et y du bloc
    int2 get_coord() const;

    /**
     * @brief Renvoie la longueur du bloc
     * 
     * @return uint8_t Longueur du bloc
     */
    uint8_t get_size() const;

    // Renvoie l'orientation du bloc
    Orientation get_orientation() const;

    /**
     * @brief Renvoie les données condensées de la structure
     * 
     * @return char 
     */
    char get_raw() const;

    // Modifie les données en fonction des paramètres
    // _x € [0; 5]
    // _y € [0; 5]
    // _size: false : taille 2    true : taille 3
    // Renvoie les données du bloc crée
    char& set_data(char _x, char _y, bool _size, Orientation _orientation);

    /**
     * @brief Modifie les données avec celle passées en paramètre
     * 
     * @param raw_data représentation condensé d'un bloc
     * @return char& 
     */
    char& set_data(char raw_data);

    friend std::ostream& operator<<(std::ostream& os, Bloc& a);

    static bool test();

private:
    // Représentation compacte des données:
    // bits: | 7 6 5 | 4 3 2 | 1 | 0
    //       |coord x|coord y|lgr|h/v
    //
    // si lgr vaut 0: longueur = 2, sinon 3
    // si h/v vaut 0: horizontal, sinon vertical
    char m_data;

};

#endif