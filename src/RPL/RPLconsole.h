#ifndef __RPL_CONSOLE__
#define __RPL_CONSOLE__

#include <iostream>
#include <string>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <cassert>
#include <stdlib.h>
#include <stdarg.h>

#include <chrono>

#include "RPLcolor.h"


namespace RPL {

enum CONSOLE_MODE {
    CONSOLE_COMPACT = 1,
    CONSOLE_SPACED = 2,
    CONSOLE_BORDERLESS = 4,
    CONSOLE_BORDERED = 8
};

enum CONSOLE_COLOR_MODE {
    CONSOLE_COLOR_TEXT,
    CONSOLE_COLOR_BACKGROUND
};

///@brief Fenetre dans la console
class consoleWindow {
public:

    /**
     * @brief Créer une fenetre de dimension (w x h)
     * 
     * @param w largeur de la fenetre
     * @param h hauteur de la fenetre
     */
    consoleWindow(unsigned int w, unsigned int h, const std::string& title="", unsigned char console_mode = 0, int _framerate = 60);

    /**
     * @brief Détruit la fenetre et libère la mémoire
     * 
     */
    ~consoleWindow();

    /**
     * @brief Défini la couleur de remplissage
     * 
     * @param col Color
     */
    void set_color(Color col);
    /**
     * @brief Défini la couleur de remplissage
     * 
     * @param r unsigned char
     * @param g unsigned char
     * @param b unsigned char
     */
    void set_color(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Défini le mode de remplissage
     * 
     * @param color_background CONSOLE_COLOR_TEXT CONSOLE_COLOR_BACKGROUND
     */
    void set_color_mode(int color_background);

    /**
     * @brief Ajoute au buffer le caractère c aux coordonnées x, y
     * 
     * @param x int
     * @param y int
     * @param c char | par défaut: ' '
     */
    void print_char(int x, int y, char c=' ');

    /**
     * @brief Ajoute la chaine de caractère au buffer à partir des coordonnées x, y
     * Si la chaine dépasse de la fenetre, elle se continuera sur la ligne suivante
     * 
     * @param x int | x € [0; dimx[
     * @param y int | y € [0; dimy[
     * @param c const char*
     */
    void print_char(int x, int y, const char* c);

    /**
     * @brief Ajoute la chaine de caractère au buffer à partir des coordonnées x, y
     * Si la chaine dépasse de la fenetre, elle se continuera sur la ligne suivante 
     *
     * @param x int | x € [0; dimx[
     * @param y int | y € [0; dimy[
     * @param string std::string
     */
    void print_char(int x, int y, const std::string& string);


    /**
     * @brief Rempli le buffer par des espace de couleur 
     * current_color avec le mode current_color_mode
     */
    void clear_window();

    /**
     * @brief Affiche le buffer sur la console
     * 
     */
    void draw_window() const;

    /**
    * @brief Récupère la touche appuyé par l'utilisateur
    * 
    * @return char Le code ASCII correspondant à l'appui utilsiateur
    */
    char getch();

    /**
     * @brief Défini le nombre d'image par seconde à 
     * laquelle l'application tourne
     */
    void set_framerate(int fps);

    /**
     * @brief S'assure que l'application tourne avec le bon
     * nombre de FPS, défini avec set_framerate
     *
     * @return bool true: une frame est passée  false: la frame n'est pas encore passée
     */
    bool handle_framerate();

private:

    struct Cell {
        Color col;
        char c;
        unsigned char col_mode;
    };

    int dimx, dimy;
    Cell* window;
    Color current_color;

    ///Mode de colorisation
    ///38: texte | 48: arrière-plan
    unsigned char current_color_mode;

    ///Dernier temps d'horloge enregistrer
    ///Utilisé pour la gestion des framerates
    std::chrono::steady_clock::time_point last_update;

    ///Nombre d'image par seconde
    int framerate;
    
    ///Ajoute un espace entre chaque caractère de la fenetre
    bool spaced;

    ///Affiche une bordure à la fenetre
    bool bordered;

    ///Titre de la fenetre
    std::string title;

    /**
     * @brief Vérifie si une touche est appuyée
     * 
     * @return int 
     */
    int kbhit();

    /// @brief configure la saisie : ne pas afficher les caracteres tapes
    void termInit();
};



}

#endif