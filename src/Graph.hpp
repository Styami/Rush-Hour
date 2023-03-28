#ifndef __GRAPH__
#define __GRAPH__
#include <queue>
#include <iostream>
#include "Sommets.hpp"
#include <unordered_map>




class Graph{
    private:
        Sommets* m_noeud;
        std::unordered_map<const Plateau&,Sommets*, Plateau::Thash, Plateau::Tequal> m_hash_map;
        std::queue<Sommets*> m_file_noeud;

        void generer(Sommets* node);
    public:
        Graph(Sommets* node);

        const Sommets* parcours();
        static void test();

        ~Graph();

};
#endif