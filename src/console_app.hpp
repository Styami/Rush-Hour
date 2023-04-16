#ifndef __AFFICHAGE__
#define __AFFICHAGE__

#include "RPL/RPLconsole.hpp"
#include "Graph.hpp"
#include <string>
#include <vector>

class Window {
public:
    Window(Graph* graph = nullptr);
    ~Window();

    /**
     * @brief Main loop, s'occupe de tout
     * 
     */
    void main_loop();

    static void test();

private:
    struct Color {
        unsigned char r, g, b;
    };
    RPL::consoleWindow m_window;

    bool m_is_running;

    std::size_t m_color_count;
    Color* m_block_color;

    enum MENU_ENTRY {
        choix_action, choix_fichier, choix_difficulte, jeu
    } m_menu_entry;
    int8_t m_menu_selection;

    std::vector<std::string> m_liste_fichiers;

    // Permet d'afficher petit à petit un texte s'il est trop long
    uint8_t m_string_display_offset;

    Graph* m_graph;
    
    std::vector<std::unique_ptr<Bloc>> m_animation;
    std::size_t m_frame, m_anim_block_count;
    
    /**
     * @brief Définit la palette de couleur pour la ligne de menu actuelle
     * selon si elle est sélectionnée ou non
     * @param current_entry ligne de menu actuelle
     */
    void determiner_palette(int current_entry);

    /**
     * @brief Détermine dans quel menu l'utilisateur à choisi d'entrer
     * 
     */
    void determiner_menu_select();

    /**
     * @brief Retourne le nombre de choix max du menu
     * 
     */
    uint8_t determienr_menu_choix_max();

    /**
     * @brief Affiche le plateau
     * 
     * @param plateau 
     */
    void dessiner_plateau(const Bloc* blocks_array, std::size_t blocks_count);

    /**
     * @brief Affiche les menus
     * 
     */
    void dessiner_menu();

    /**
     * @brief Dessine le menu de choix d'action (1er menu)
     * 
     */
    void dessiner_choix_action();

    /**
     * @brief Dessine le menu pour choisir un fichier à charger
     * 
     */
    void dessiner_choix_fichier();

    /**
     * @brief Dessiner le menu pour choisir une difficulté de niveau
     * 
     */
    void dessiner_choix_difficulte();

    /**
     * @brief Gère les entrées utilisateurs
     */
    void handle_input();
};

#endif