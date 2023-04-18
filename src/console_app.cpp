#include "console_app.hpp"

#include "RPL/RPLconsole.hpp"
#include "utils.hpp"
#include "bloc.hpp"

#include <memory>
#include <vector>
#include <filesystem>
#include <algorithm>

static void update_liste_fichier(std::vector<std::string>& liste)
{
    int level_count;
    liste.clear();
    // Listage de tous les fichiers de niveaux
    for(const auto& file : std::filesystem::directory_iterator("data/niveaux"))
    {
        // Retire l'extension du fichier et le chemin vers le fichier (pour n'avoir que le nom du niveau)
        std::string raw_path = file.path();
        std::size_t start_index = raw_path.find_last_of('/') + 1;
        std::size_t end_index = raw_path.find_last_of('.');
        liste.push_back(raw_path.substr(start_index, end_index - start_index));
    }
}

Window::Window(Graph* graph) :
    m_window(12, 12, "Rush Hour", RPL::CONSOLE_BORDERED | RPL::CONSOLE_SPACED, 5),
    m_is_running(true),
    m_color_count(16),
    m_block_color(new Color[16]),
    m_menu_entry(choix_action),
    m_menu_selection(0),
    m_string_display_offset(0),
    m_graph(graph)
{
    // Génération des couleurs aléatoire
    srand(time(NULL));
    for(std::size_t i = 0; i < m_color_count; i++) {
        m_block_color[i].r = 100 + random() % 156;
        m_block_color[i].g = 100 + random() % 156;
        m_block_color[i].b = 100 + random() % 156;
    }
    update_liste_fichier(m_liste_fichiers);
    level_count = m_liste_fichiers.size();
}

Window::~Window() 
{
    delete [] m_block_color;
}

void Window::main_loop()
{
    const Bloc* cur_frame;
    while(m_is_running)
    {
        if(m_window.handle_framerate())
        {
            m_window.set_color(0, 0, 0);
            m_window.clear_window();
            
            if(m_menu_entry == jeu)
            {
                cur_frame = m_animation[m_frame];
                dessiner_plateau(cur_frame, m_anim_block_count);
                if(++m_frame > m_animation.size() - 1)
                    m_frame = 0;   
            }
            else
                dessiner_menu();

            m_window.draw_window();

            m_string_display_offset++;
        }
        handle_input();
    }
}

void Window::creer_animation(const Sommets* sommet_resultat)
{
    m_animation.clear();
    m_anim_block_count = sommet_resultat->get_plateau()->get_block_count();
    m_winning_block_index = sommet_resultat->get_plateau()->get_winning_block();
    // On va récupérer tous les antécédents de ce noeud jusqu'à la racine, en les insérant dans l'animation
    do {
        m_animation.push_back(std::move(sommet_resultat->get_plateau()->get_block_array()));
        sommet_resultat = sommet_resultat->m_precedent.lock().get();
    } while(!sommet_resultat->m_precedent.expired() 
          && sommet_resultat->m_precedent.lock().get() != sommet_resultat);

    // On remet l'animation dans le bon ordre
    std::reverse(m_animation.begin(), m_animation.end());
    m_frame = 0;
}

void Window::dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count) 
{
    m_window.set_color_mode(RPL::CONSOLE_COLOR_BACKGROUND);
    uint2 pos;

    for(std::size_t i = 0; i < blocks_count; i++) {
        pos = blocks_array[i].get_coord() * 2;

        if(m_winning_block_index != i)
            m_window.set_color(m_block_color[i].r, m_block_color[i].g, m_block_color[i].b);
        else
            m_window.set_color(255, 255, 0);

        for(int j = 0; j < blocks_array[i].get_size(); j++) {
            m_window.print_char(pos.x, pos.y);
            m_window.print_char(pos.x + 1, pos.y);
            m_window.print_char(pos.x + 1, pos.y + 1);
            m_window.print_char(pos.x, pos.y + 1);

            if(blocks_array[i].get_orientation() == Orientation::horizontal)
                pos.x+=2;
            else
                pos.y+=2;
        }
    }
}


void Window::determiner_palette(int current_entry) 
{
    if(m_menu_selection == current_entry)
        m_window.set_color(255, 255, 255);
    else 
        m_window.set_color(128, 128, 128);
}

void Window::determiner_menu_select() 
{
    switch(m_menu_entry)
    {
    case choix_action:
        switch(m_menu_selection) {
        case 0:
            m_menu_entry = choix_fichier;
            m_menu_selection = 0;
        break;
        case 1:
            m_menu_entry = choix_difficulte;
            m_menu_selection = 0;
        break;
        case 2:
            m_is_running = false;
        break;
        }
    break;
    case choix_fichier:
        m_menu_entry = jeu;
        {
        // Crée un plateau à partir du fichier sélectionné
        m_graph->charger_plateau(
            std::make_unique<Plateau>("data/niveaux/" + m_liste_fichiers[m_menu_selection] + ".rh")
        );
        
        // On lance un parcours du graph, et on génère l'animation avec
        creer_animation(m_graph->parcours(false, true)[0].get());
        }
    break;
    case choix_difficulte:
        m_menu_entry = choix_action;
        {
        std::vector<std::shared_ptr<Sommets>> resultat;

        // On génère des plateaux tant qu'il n'existe pas de solution
        do {
            std::unique_ptr<Plateau> p = std::make_unique<Plateau>();
            // Tant que le plateau n'est pas valide on le regénère
            while(!p->generer_aleatoirement(8 + m_menu_selection * 3)); // Facile: 8  Moyen: 11  Difficile: 14
            m_graph->charger_plateau(std::move(p));

            resultat = m_graph->parcours(true, true);
        } while(resultat.size() == 0);

        std::shared_ptr<Sommets> res = std::move(m_graph->parcours(true, false, resultat)[0]);
        
        res->get_plateau()->sauvegarder("data/niveaux/niveau_" + std::to_string(level_count) + ".rh");
        creer_animation(
            res.get()
        );
        }
    break;
    default:
    break;
    }
}

uint8_t Window::determienr_menu_choix_max()
{
    switch(m_menu_entry) {
    case choix_action:
        return 2; // charger, generer, quitter
        break;
    case choix_fichier:
        return m_liste_fichiers.size() - 1;
        break;
    case choix_difficulte:
        return 2; // facile, moyen, difficile
        break;
    default:
        return 0;
        break;
    }
}

void Window::dessiner_menu() 
{
    m_window.set_color_mode(RPL::CONSOLE_COLOR_TEXT);

    if(m_menu_entry == choix_action) 
        dessiner_choix_action();
    else if(m_menu_entry == choix_fichier)
        dessiner_choix_fichier();
    else
        dessiner_choix_difficulte();    
}

// Permet de faire scroller le texte
static int handle_string_offset(uint8_t& cur_offset, int lg_string)
{
    if(lg_string <= 10)
        return 0;
    
    int max_offset = lg_string - 10;

    int scrolling_timer = 10;

    if(cur_offset < scrolling_timer)
        return 0;
    else if(cur_offset < scrolling_timer + max_offset)
        return cur_offset - scrolling_timer;
    else if(cur_offset < 2 * scrolling_timer + max_offset)
        return max_offset;
    else
        cur_offset = 0;
    return cur_offset;
}

// Permet de faire scroller la liste
static int handle_liste_scrolling(int cur_select)
{
    return fmax(0, cur_select - 9);
}

// Détermine le x où commencer à afficher le texte pour qu'il soit centré
static int determiner_x_debut(std::size_t lg_string)
{
    return 1 + (11 - lg_string) * 0.5;
}

// Détermine le y où commencer à afficher le texte pour que la liste soit centré
static int determiner_y_debut(int cur_iter, int lg_liste)
{
    if(lg_liste >= 9)
        return 1 + cur_iter;
    else
        return 1 + (10 - lg_liste) * 0.5 + cur_iter;
}


void Window::dessiner_choix_action() 
{
    int start = determiner_x_debut(7);
    determiner_palette(0);
    m_window.print_char(start, 4, "Charger");
    determiner_palette(1);
    m_window.print_char(start, 5, "Generer");
    determiner_palette(2);
    m_window.print_char(start, 7, "Quitter");
}

void Window::dessiner_choix_fichier()
{
    std::string s;
    int debut = handle_liste_scrolling(m_menu_selection);
    int fin = debut + fmin(10, m_liste_fichiers.size());
    int offset;
    for(int i = debut; i < fin; i++)
    {
        // récupère la couleur d'affichage
        determiner_palette(i);

        // Si la ligne est sélectionné, fait défiler le texte trop long
        if(m_menu_selection == i)
            offset = handle_string_offset(m_string_display_offset, m_liste_fichiers[i].size());
        else
            offset = 0;

        s = m_liste_fichiers[i].substr(
                offset, 
                10);

        // Inscrit le texte
        m_window.print_char(
            determiner_x_debut(s.length()), 
            determiner_y_debut(i - debut, m_liste_fichiers.size()),
            s);
    }
}

void Window::dessiner_choix_difficulte() 
{
    determiner_palette(0);
    m_window.print_char(determiner_x_debut(6), 4, "Facile");
    determiner_palette(1);
    m_window.print_char(determiner_x_debut(5), 5, "Moyen");
    determiner_palette(2);
    m_window.print_char(determiner_x_debut(9), 6, "Difficile");
}

void Window::handle_input() 
{
    char c = m_window.getch();
    
    switch(c) {
        case 'z':
            if(--m_menu_selection < 0)
                m_menu_selection = 0;
            break;
        case 's':
            if(++m_menu_selection > determienr_menu_choix_max())
                m_menu_selection = determienr_menu_choix_max();
            break;
        case 'v':
            determiner_menu_select();
            break;
        case 'q':
            m_is_running = false;
            break;
    }
}


void Window::test() 
{
    Window w;

    Plateau p("data/test/test_data_save.rh");
    //Plateau p("data/niveaux/niveau_0.rh");


    w.m_window.set_color_mode(RPL::CONSOLE_COLOR_BACKGROUND);

    w.m_window.set_color(0, 0, 0);
    w.m_window.clear_window();
    w.dessiner_plateau(p.get_block_array(), p.get_block_count());
    w.m_window.draw_window();
    
    std::vector<std::unique_ptr<Plateau>> pp = p.get_neighbours();

    // for(std::size_t i = 0; i < pp.size(); i++) {
    //    getchar();
    //    w.m_window.set_color(0, 0, 0);
    //    w.m_window.clear_window();
    //    w.dessiner_plateau(pp[i]->get_block_array(), pp[i]->get_block_count());
    //    w.m_window.draw_window();
    // }

    p.generer_aleatoirement(13);

    getchar();
    w.m_window.set_color(0, 0, 0);
    w.m_window.clear_window();
    w.dessiner_plateau(p.get_block_array(), p.get_block_count());
    w.m_window.draw_window();

    p.generer_aleatoirement(10);

    getchar();
    w.m_window.set_color(0, 0, 0);
    w.m_window.clear_window();
    w.dessiner_plateau(p.get_block_array(), p.get_block_count());
    w.m_window.draw_window();
}