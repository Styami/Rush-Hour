#ifndef __GRAPH__
#define __GRAPH__
#include <queue>
#include <vector>
#include <iostream>
#include "Sommets.hpp"

using namespace std;

template <class T>
class Graph{
    private:
        vector<Sommets<T>*> m_noeud;
        queue<Sommets<T>*> m_file_noeud;

    public:
        Graph(vector<Sommets<T>*> init_val);

        void parcours(T val);

};



#include "Graph.cpp"
#endif