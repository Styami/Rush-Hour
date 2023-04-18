#ifndef __GRAPH__
#define __GRAPH__
#include <memory>
#include <queue>
#include <iostream>
#include "Sommets.hpp"
#include <unordered_map>
#include <assert.h>




class Graph{
    private:
        std::weak_ptr<Sommets> m_racine;
        std::unordered_map<Plateau*, std::shared_ptr<Sommets>, Plateau::Thash, Plateau::Tequal> m_hash_map;
        std::queue<std::shared_ptr<Sommets>> m_file_noeud;

        void generer(std::shared_ptr<Sommets> node);
        void init_parcours(bool generate_graph, std::vector<std::shared_ptr<Sommets>> solutions = {nullptr});
        void ajouter_voisin_file(std::shared_ptr<Sommets> sommet_courant, bool generate_graph);
    public:
        Graph();
        Graph(std::shared_ptr<Sommets> node);
        ~Graph();

        void charger_plateau(std::unique_ptr<Plateau> plateau);

        /**
         * @brief Parcours le graph, soit en le générant au passage, soit en résolvant le graph déjà existant
         * Dans le second cas, il faudra passer une liste des noeuds de bases pour trouver le point le plus éloigné de ceux-ci
         * @param find_all Est-ce qu'on doit trouver tous les noeuds solutions
         * @param generate_graph Est-ce qu'on doit générer le graph
         * @param solutions Liste des solutions de base pour générer un nouveau niveau
         * @return std::vector<std::shared_ptr<Sommets>> Liste des solutions trouvées par l'algorithme
         */
        std::vector<std::shared_ptr<Sommets>> parcours(bool parcours_complet, bool generate_graph, std::vector<std::shared_ptr<Sommets>> solutions = {});
        static void test();


};
#endif