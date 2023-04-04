#include "affichage.hpp"

#include "RPL/RPLconsole.h"
#include "bloc.hpp"
#include "utils.hpp"

Window::Window(std::size_t bloc_count) :
    m_window(6, 6, "Rush Hour", 60, RPL::CONSOLE_BORDERED | RPL::CONSOLE_SPACED),
    m_blocks_count(bloc_count),
    m_block_color(new Color[m_blocks_count])
{
    m_window.set_color_mode(RPL::CONSOLE_COLOR_BACKGROUND);

    for(int i = 0; i < m_blocks_count; i++) {
        m_block_color[i].r = random() % 256;
        m_block_color[i].g = random() % 256;
        m_block_color[i].b = random() % 256;
    }
}

void Window::dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count) {
    int2 pos;

    m_window.set_color(0, 0, 0);
    m_window.clear_window();

    for(std::size_t i = 0; i < blocks_count; i++) {
        pos = blocks_array[i].get_coord();

        m_window.set_color(m_block_color[i].r, m_block_color[i].g, m_block_color[i].b);

        for(int j = 0; j < blocks_array[i].get_size() * 2 - 1; j++) {
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
    Window w(5);

    Bloc plateau[5];
    plateau[0].set_data(0, 0, false, Orientation::horizontal);
    plateau[1].set_data(3, 0, true, Orientation::vertical);
    plateau[2].set_data(0, 5, true, Orientation::horizontal);
    plateau[3].set_data(5, 4, false, Orientation::vertical);
    plateau[4].set_data(2, 2, false, Orientation::horizontal);

    w.dessiner_plateau(plateau, 5);
}