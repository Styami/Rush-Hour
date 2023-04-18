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

    Bloc(const Bloc& b);
    Bloc(Bloc&& b);

    /**
     * @brief Assignement par copie
     * 
     * @param b Bloc à copier
     * @return Bloc& Référence vers le bloc
     */
    Bloc& operator=(const Bloc& b);

    /**
     * @brief Assignement depuis une donnée brute
     * 
     */
    char& operator=(char raw_data);

    /**
     * @brief Renvoie un int2 contenant la position x et y du bloc
     * 
     * @return uint2 
     */
    uint2 get_coord() const { return uint2( (m_data & 0b11100000) >> 5, (m_data & 0b00011100) >> 2 ); };

    /**
     * @brief Renvoie la longueur du bloc
     * 
     * @return uint8_t Longueur du bloc
     */
    uint8_t get_size() const { return ((m_data & 0b00000010) == 2 ? 3 : 2); };

    /**
    * @brief Renvoie l'orientation du bloc
    * 
    * @return Orientation 
    */
    Orientation get_orientation() const { return (m_data & 0b00000001) == 0 ? Orientation::horizontal : Orientation::vertical; };

    /**
     * @brief Renvoie les données condensées de la structure
     * 
     * @return char 
     */
    char get_raw() const { return m_data; };

    // Modifie les données en fonction des paramètres
    // _x € [0; 5]
    // _y € [0; 5]
    // _size: false : taille 2    true : taille 3
    // Renvoie les données du bloc crée
    char& set_data(char _x, char _y, bool _size, Orientation _orientation);

    /**
     * @brief Modifie les coordonnées du bloc
     * 
     * @param new_coord 
     */
    void set_coord(uint2 new_coord);

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