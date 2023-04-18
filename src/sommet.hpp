#ifndef __SOMMETS__
#define __SOMMETS__

#include "plateau.hpp"


#include <vector>
#include <memory>


class Sommets{
public:
    bool m_traite;
    std::weak_ptr<Sommets> m_precedent;
    mutable int m_distance;

    Sommets(std::unique_ptr<Plateau> plateau);
    Sommets(const Sommets& s);
    Sommets(Sommets&& s);

    Sommets& operator=(const Sommets& s);
    Sommets& operator=(Sommets&& s);

    ~Sommets();

    /**
        * @brief Créer le lien entre les deux sommets passés en paramètre
        * 
        * @param sommet_A Premier sommet
        * @param sommet_B Deuxième sommet
        * @param poids Poids du lien (par défaut 1)
        */
    static void creer_lien(const std::shared_ptr<Sommets>& sommet_A, const std::shared_ptr<Sommets>& sommet_B, int poids = 1);

    /**
        * @brief Renvoie le plateau associé au sommet
        * 
        * @return std::shared_ptr<Plateau> Plateau associé au sommet
        */
    std::shared_ptr<Plateau> get_plateau() const { return m_plateau; };

    /**
        * @brief Récupère les instances des voisins déjà existantes depuis les liens entre sommets
        * 
        * @return std::vector<std::shared_ptr<Sommets>> Tableau de voisins
        */
    std::vector<std::shared_ptr<Sommets>> get_voisins() const;

    /**
        * @brief Récupère les voisins générés par la classe plateau
        * 
        * @return std::vector<std::unique_ptr<Plateau>> Tableau de voisins à déplacer dans la hash_map
        */
    std::vector<std::unique_ptr<Plateau>> generer_voisins() const { return m_plateau->get_neighbours(); };

    

    static void test();

private:
    struct Lien{
        std::weak_ptr<Sommets> voisin;
        int poids;
    };
    std::shared_ptr<Plateau> m_plateau;
    std::vector<Lien> m_chemin_voisins;
};
#endif