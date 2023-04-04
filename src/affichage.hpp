#ifndef __AFFICHAGE__
#define __AFFICHAGE__

#include "RPL/RPLconsole.h"
#include "bloc.hpp"

class Window {
public:
    Window(std::size_t bloc_count);

    /**
     * @brief Définit le plateau à afficher
     * 
     * @param plateau 
     */
    void dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count);

    static void test();

private:
    struct Color {
        unsigned char r, g, b;
    };
    RPL::consoleWindow m_window;

    std::size_t m_blocks_count;
    Color* m_block_color;
};

#endif