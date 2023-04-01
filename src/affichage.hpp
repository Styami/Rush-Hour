#ifndef __AFFICHAGE__
#define __AFFICHAGE__

#include "RPL/RPLconsole.h"
#include "bloc.hpp"

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
    void dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count);

    static void test();

private:
    RPL::consoleWindow m_window;

    const Bloc* m_blocks_array;
    std::size_t m_blocks_count;
};

#endif