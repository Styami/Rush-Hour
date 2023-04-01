#include "affichage.hpp"

#include "RPL/RPLconsole.h"
#include "bloc.hpp"
#include "utils.hpp"

Window::Window() :
    m_window(17, 17, "Rush Hour", 60, RPL::CONSOLE_BORDERED | RPL::CONSOLE_SPACED),
    m_blocks_array(nullptr), m_blocks_count(0)
{}

void Window::clear() {
    m_window.set_color(0, 0, 0);
    m_window.clear_window();
}

void Window::dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count) {
    int2 pos;

    for(std::size_t i = 0; i < blocks_count; i++) {
        pos = blocks_array[i].get_coord();

        m_window.set_color(128 + random() % 128, 128 + random() % 128, 128 + random() % 128);

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
    Window w;

    w.clear();

    Bloc plateau[5];
    plateau[0].set_data(0, 0, false, Orientation::horizontal);
    plateau[1].set_data(3, 0, true, Orientation::vertical);
    plateau[2].set_data(0, 5, true, Orientation::horizontal);
    plateau[3].set_data(5, 4, false, Orientation::vertical);
    plateau[4].set_data(2, 2, false, Orientation::horizontal);

    w.dessiner_plateau(plateau, 5);

}