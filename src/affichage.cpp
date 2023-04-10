#include "affichage.hpp"

#include "RPL/RPLconsole.h"
#include "bloc.hpp"
#include "utils.hpp"
#include "plateau.hpp"

#include <memory>
#include <vector>

Window::Window(std::size_t color_count) :
    m_window(6, 6, "Rush Hour", 60, RPL::CONSOLE_BORDERED | RPL::CONSOLE_SPACED),
    m_color_count(color_count),
    m_block_color(new Color[m_color_count])
{
    m_window.set_color_mode(RPL::CONSOLE_COLOR_BACKGROUND);

    srand(time(NULL));
    for(std::size_t i = 0; i < m_color_count; i++) {
        m_block_color[i].r = 100 + random() % 156;
        m_block_color[i].g = 100 + random() % 156;
        m_block_color[i].b = 100 + random() % 156;
    }
}

Window::~Window() 
{
    delete [] m_block_color;
}

void Window::dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count) {
    uint2 pos;

    m_window.set_color(0, 0, 0);
    m_window.clear_window();

    for(std::size_t i = 0; i < blocks_count; i++) {
        pos = blocks_array[i].get_coord();

        m_window.set_color(m_block_color[i].r, m_block_color[i].g, m_block_color[i].b);

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
    Window w(10);

    Plateau p("data/test_data_save.rh");
    w.dessiner_plateau(p.get_block_array(), p.get_block_count());
    
    std::vector<std::unique_ptr<Plateau>> pp = p.get_neighbours();

    for(std::size_t i = 0; i < pp.size(); i++) {
       getchar();
       w.dessiner_plateau(pp[i]->get_block_array(), pp[i]->get_block_count());
    }
}