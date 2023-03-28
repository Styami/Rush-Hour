#ifndef __AFFICHAGE__
#define __AFFICHAGE__

#include "plateau.hpp"

class Window {
public:
    Window();

    /**
     * @brief Clear la fenêtre d'affichage
     * 
     */
    void clear();

    /**
     * @brief Définit le plateau à afficher
     * 
     * @param plateau 
     */
    void set_active_plateau(const Plateau& plateau);

    /**
     * @brief Affiche le plateau
     * 
     */
    void draw();


private:
    const Plateau& plateau;
};

#endif