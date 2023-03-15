#include "Graph.hpp"


template <class T>
Graph<T>::Graph(/*vector<Sommets<T>*> init_val,*/ std::unordered_map<T, Sommets<T>*> map_init) :
    m_noeud(map_init.begin()->second),
    m_hash_map(map_init)
{}

template <class T>
void Graph<T>::generer(Sommets<T>* val){
    std::vector<Sommets<T>*> tab_tmp=val->generer();
    for(Sommets<T>* t: tab_tmp){
        auto it=m_hash_map.find(t->get_element());
        if(it==m_hash_map.end()){
            m_hash_map.insert(std::pair<T,Sommets<T>*>(t->get_element(), t));
            val->link(t);
            //std::cout << t->get_element() << std::endl;
            //for(T p: val->m_voisins)
            //    std::cout << "valeur voisin" << p << std::endl;
        }
        else{
            val->link(it->second);
        }
        m_file_noeud.push(t);
    }
}

template <class T>
void Graph<T>::parcours(T val){
    m_file_noeud.push(m_noeud);
    while(!m_file_noeud.empty()){
        Sommets<T>* current_noeud=m_file_noeud.front();
        m_file_noeud.pop();
        if(!current_noeud->m_traite){
            if(current_noeud->get_element()==val){
                std::cout << "valeur trouvée" << std::endl;
                return;
            }
            generer(current_noeud);
            std::cout << current_noeud->get_element() << std::endl;
            current_noeud->m_traite=true;
        }
    }
    std::cout << "la valeur n'a pas été trouvé" << std::endl;
}


