#ifndef __GRAPH__
#define __GRAPH__
#include <queue>
#include <vector>
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
        Graph(/*std::vector<Sommets<T>*> init_val,*/ std::unordered_map<T, Sommets<T>*> map_init);

        void parcours(T val);
        void generer(Sommets<T>* val);
        bool insertion(T val);

};



#include "Graph.cpp"
#endif