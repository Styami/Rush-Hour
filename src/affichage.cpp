#include "affichage.hpp"

#include "RPL/RPLconsole.h"
#include "bloc.hpp"
#include "utils.hpp"
#include "plateau.hpp"

Window::Window() :
    m_window(6, 6, "Rush Hour", 60, RPL::CONSOLE_BORDERED | RPL::CONSOLE_SPACED),
    m_blocks_array(nullptr), m_blocks_count(0)
{
    m_window.set_color_mode(RPL::CONSOLE_COLOR_BACKGROUND);
}

void Window::dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count) {
    int2 pos;

    m_window.set_color(0, 0, 0);
    m_window.clear_window();

    for(std::size_t i = 0; i < blocks_count; i++) {
        pos = blocks_array[i].get_coord();

        m_window.set_color(random() % 256, random() % 256, random() % 256);

        for(int j = 0; j < blocks_array[i].get_size(); j++) {
            m_window.print_char(pos.x, pos.y);

            if(blocks_array[i].get_orientation() == Orientation::horizontal)
                pos.x++;
            else
                pos.y++;
        }
    }

    m_window.draw_window();
}

void Window::test() {
    Window w;

    // Bloc plateau[5];
    // plateau[0].set_data(0, 0, false, Orientation::horizontal);
    // plateau[1].set_data(3, 0, true, Orientation::vertical);
    // plateau[2].set_data(0, 5, true, Orientation::horizontal);
    // plateau[3].set_data(5, 4, false, Orientation::vertical);
    // plateau[4].set_data(2, 2, false, Orientation::horizontal);
    Plateau p("data/niveau0.rh");
    w.dessiner_plateau(p.get_block_array(), p.get_block_count());
}