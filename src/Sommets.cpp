#include "Sommets.hpp"
#include <cstddef>
#include <cstdlib>
#include <time.h>


template<class T>
Sommets<T>::Sommets(T element){
    m_element=element;
}

template<class T>
void Sommets<T>::link(Sommets<T> som){
    m_voisins.push_back(som);
    som.push_back(m_voisins);
}

template<class T>
Sommets<T> Sommets<T>::generer(){
    // srand(time(NULL));
    // for (int i=0; i<3; i++) {
    //     voisins.push_back(rand() % 100);
    // }
    
}