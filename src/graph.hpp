#ifndef __GRAPH__
#define __GRAPH__

#include "sommet.hpp"

#include <memory>
#include <queue>
#include <unordered_map>

class Graph{
public:
    Graph();
    Graph(std::shared_ptr<Sommets> node);
    ~Graph();

    /**
        * @brief Permet de charger un plateau comme racine dans le graph.
        * Réinitialise le graph
        *
        * @param plateau Nouvelle racine
        */
    void charger_plateau(std::unique_ptr<Plateau> plateau);

    /**
        * @brief Parcours le graph, soit en le générant au passage, soit en résolvant le graph déjà existant
        * Dans le second cas, il faudra passer une liste des noeuds de bases pour trouver le point le plus éloigné de ceux-ci
        * @param find_all Est-ce qu'on doit trouver tous les noeuds solutions
        * @param generer_graph Est-ce qu'on doit générer le graph
        * @param solutions Liste des solutions de base pour générer un nouveau niveau
        * @return std::vector<std::shared_ptr<Sommets>> Liste des solutions trouvées par l'algorithme
        */
    std::vector<std::shared_ptr<Sommets>> parcours(bool parcours_complet, bool generer_graph, std::vector<std::shared_ptr<Sommets>> solutions = {});


    static void test();

private:
    std::weak_ptr<Sommets> m_racine;

    /**
        * @brief La hash_map sera la détentrice des Sommets
        */
    std::unordered_map<Plateau*, std::shared_ptr<Sommets>, Plateau::Thash, Plateau::Tequal> m_hash_map;
    std::queue<std::shared_ptr<Sommets>> m_file_noeud;

    /**
        * @brief Récupère les voisins généré depuis le sommet node
        * 
        * @param node 
        */
    void instancier_voisins(std::shared_ptr<Sommets> node);

    /**
        * @brief Initialise le graph avant de lancer le parcours en fonction du mode passé en paramètre
        * 
        * @param generer_graph Mode : définit si on veut générer le graph ou juste le parcourir
        * @param solutions liste de sommets qui remplacera la racine generer_graph est à faux
        */
    void init_parcours(bool generer_graph, std::vector<std::shared_ptr<Sommets>> solutions = {nullptr});

    /**
        * @brief Ajoute les voisins du sommet courant à la file. 
        * La source (Généré ou lu) de ces voisins dépend du mode passé en paramètre
        * 
        * @param sommet_courant Sommet dont on veut déterminer les voisins
        * @param generate_graph Mode : définit si on veut générer le graph ou juste le parcourir
        */
    void ajouter_voisin_file(std::shared_ptr<Sommets> sommet_courant, bool generate_graph);
};
#endif