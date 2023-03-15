#ifndef __SOMMETS__
#define __SOMMETS__
#include <stack>
#include <vector>
#include <ctime>
#include <cstdlib>


template<class T>
class Sommets{
    private:
        T m_element;
    public:
        Sommets(T element);

        std::vector<Sommets<T>*> m_voisins;
        
        void link(Sommets<T>* som);

        std::vector<Sommets<T>*> generer();

        T get_element();
        bool m_traite=false;
        
};
#include "Sommets.cpp"
#endif