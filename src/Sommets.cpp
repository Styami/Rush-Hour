#include "Sommets.hpp"

template<class T>
Sommets<T>::Sommets(T element) :
    m_element(element)
{}

template<class T>
void Sommets<T>::link(Sommets<T>* som){
    m_voisins.push_back(som);
    som->m_voisins.push_back(this);
}

template <class T>
T Sommets<T>::get_element(){
    return m_element;
}

template<class T>
std::vector<Sommets<T>*> Sommets<T>::generer(){
    std::srand(std::time(nullptr));
    std::vector<Sommets<T>*> tmp;
    for(int i=0; i<3; i++){
        tmp.push_back(new Sommets<T>(rand()%10));
    }
    return tmp;
}

