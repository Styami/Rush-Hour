#ifndef __GRAPH__
#define __GRAPH__
#include <queue>
#include <iostream>
#include "Sommets.hpp"
#include <unordered_map>




class Graph{
    private:
        Sommets* m_racine;
        std::unordered_map<Plateau*,Sommets*, Plateau::Thash, Plateau::Tequal> m_hash_map;
        std::queue<Sommets*> m_file_noeud;

        void generer(Sommets* node);
        void restart_parcours();
    public:
        Graph(Sommets* node);

        Sommets* parcours(bool chercher_solution);
        static void test();

        ~Graph();

};
#endif