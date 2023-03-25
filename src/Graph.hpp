#ifndef __GRAPH__
#define __GRAPH__
#include <queue>
#include <iostream>
#include "Sommets.hpp"
#include <unordered_map>



template <class T>
class Graph{
    private:
        Sommets<T>* m_noeud;
        std::unordered_map<T,Sommets<T>*> m_hash_map;
        std::queue<Sommets<T>*> m_file_noeud;

    public:
        Graph(Sommets<T>* node);

        void parcours(T val, int nb_voisins);
        void generer(Sommets<T>* node, int nb_voisins);
        static void test();

        ~Graph();

};
#include "Graph.tpp"
#endif