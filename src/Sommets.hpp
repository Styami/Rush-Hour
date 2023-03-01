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
        vector<Sommets<T>> m_voisins;
        Sommets(T element);

        void link(Sommets<T> som);

        Sommets<T> generer();

        
};

#endif