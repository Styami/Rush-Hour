#ifndef __GRAPH__
#define __GRAPH__
#include <memory>
#include <queue>
#include <iostream>
#include "Sommets.hpp"
#include <unordered_map>




class Graph{
    private:
        std::weak_ptr<Sommets> m_racine;
        std::unordered_map<Plateau*, std::shared_ptr<Sommets>, Plateau::Thash, Plateau::Tequal> m_hash_map;
        std::queue<std::shared_ptr<Sommets>> m_file_noeud;

        void generer(std::shared_ptr<Sommets> node);
        void restart_parcours();
    public:
        Graph();
        Graph(std::shared_ptr<Sommets> node);

        void charger_plateau(std::unique_ptr<Plateau> plateau);

        std::shared_ptr<Sommets> generer_lvl(std::vector<std::shared_ptr<Sommets>> solutions);
        std::vector<std::shared_ptr<Sommets>> parcours(bool chercher_solution);
        static void test();

        ~Graph();

};
#endif