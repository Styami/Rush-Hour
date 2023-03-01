#ifndef __GRAPH__
#define __GRAPH__
#include <stack>
#include <vector>

using namespace std;

template <class T>
class Graph{
    private:
        vector<T> m_noeud;
        stack<T> m_trait_noeud;

    public:
        Graph();

        void parcours();

};




#endif