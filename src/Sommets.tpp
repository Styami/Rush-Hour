template<class T>
Sommets<T>::Sommets(T element) :
    m_element(element),
    m_traite(false)
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
std::vector<Sommets<T>*> Sommets<T>::generer(int nb_voisins){
    std::vector<Sommets<T>*> tmp;
    for(int i=0; i<5; i++){
        int z =rand()%50;
        tmp.push_back(new Sommets<T>(z));
    }
    return tmp;
}

template<class T>
Sommets<T>::~Sommets(){
    m_element=0;
    m_traite=false;
    m_voisins.clear();
}

template<class T>
void Sommets<T>::test(){
    Sommets<int>* n=new Sommets<int>(5);
    assert(n->get_element()==5);
    assert(n->m_traite==false);
    Sommets<int>* an=new Sommets<int> (6);
    n->link(an);
    an=nullptr;
    assert(n->m_voisins[0]->get_element()==6);
    delete n->m_voisins[0];
    n->m_voisins.clear();
}