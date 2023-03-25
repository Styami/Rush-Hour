#ifndef __SOMMETS__
#define __SOMMETS__
#include <stack>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <assert.h>

template<class T>
class Sommets{
    private:
        T m_element;
    public:
        bool m_traite;
        std::vector<Sommets<T>*> m_voisins;
        Sommets(T element);

        void link(Sommets<T>* som);
        std::vector<Sommets<T>*> generer(int nb_voisins);
        T get_element();
        static void test();

        ~Sommets();
        
};
#include "Sommets.tpp"
#endif