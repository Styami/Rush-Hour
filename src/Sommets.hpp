#ifndef __SOMMETS__
#define __SOMMETS__
#include <stack>
#include <vector>

using namespace std;

template<class T>
class Sommets{
    private:
        T m_element;
    public:
        Sommets(T element);

        vector<Sommets<T>*> m_voisins;
        void link(Sommets<T>* som);

        Sommets<T> generer();

        T get_element();
        bool m_traite=false;
        
};
#include "Sommets.cpp"
#endif